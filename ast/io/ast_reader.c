#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../ast.h"
#include "ast_reader.h"
#include "magic_num.h"
#include "serialize.h"

#define structsize(X) sizeof(struct X)

static void status(bool debug, char* msg){
	
	if(debug){ printf("[AST] %s\n", msg); }
}

static void error(FILE* file, char* msg){
	
	if(file != NULL){ fclose(file); }
	
	printf("[AST][Error] %s\n", msg);
	exit(1);
}

struct AST* readAST(char** filenames, int count_filenames, bool debug){
	
	status(debug, "readAST");

	struct AST* ast = make(AST);
	
	ast->count_namespaces = count_filenames;
	
	int nbytes = sizeof(void*) * ast->count_namespaces;
	
	ast->namespaces = malloc(nbytes);

	for(int i = 0;i < count_filenames; i++){

		FILE* file = fopen(filenames[i], "r");
		
		if(file == NULL){
			printf("%s\n", filenames[i]);
			error(NULL, "Could not open file");
		}

		ast->namespaces[i] = readNamespace(file, debug);

		fclose(file);
	}
	
	return ast;
}

struct Namespace* readNamespace(FILE* file, bool debug){
	
	status(debug, "readNamespace");
	
	magic_num_require(MAGIC_NAMESPACE, file);
	
	struct Namespace* ns = make(Namespace);

	char* tmpSrcPath = deserialize_string(file);
	strcpy(ns->srcPath, tmpSrcPath);
	free(tmpSrcPath);
	
	char* tmpASTFilename = deserialize_string(file);
	strcpy(ns->ast_filename, tmpASTFilename);
	free(tmpASTFilename);
	
	tmpSrcPath = deserialize_string(file);
	strcpy(ns->name, tmpSrcPath);
	free(tmpSrcPath);
	
	ns->count_methods = deserialize_int(file);

	//read methods
	ns->methods = malloc(sizeof(void*)*(ns->count_methods));
	
	for(int i=0; i < ns->count_methods; i++){
		
		ns->methods[i] = readMethod(file, debug);
	}
	
	//read structs
	ns->count_structs = deserialize_int(file);
	
	ns->structs = malloc(sizeof(void*)*(ns->count_structs));
	
	for(int i=0;i < ns->count_structs; i++){
		
		ns->structs[i] = readStructDecl(file, debug);
	}
	
	magic_num_require(MAGIC_END_NAMESPACE, file);
	
	return ns;
}
struct Method* readMethod(FILE* file, bool debug){
	
	status(debug, "readMethod");
	
	magic_num_require(MAGIC_METHOD, file);
	
	struct Method* m  = make(Method);

	m->isPublic       = deserialize_int(file);
	m->hasSideEffects = deserialize_int(file);

	char* tmp = deserialize_string(file);
	strcpy(m->name, tmp);
	
	free(tmp);

	m->returnType = readType(file, debug);
	
	m->count_args = deserialize_int(file);
	
	m->args = malloc(sizeof(void*)*(m->count_args));

	for(int i = 0;i < m->count_args;i++){
		m->args[i] = readDeclArg(file, debug);
	}

	m->block = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_METHOD, file);
	
	return m;
}
struct StructDecl* readStructDecl(FILE* file, bool debug){
	
	status(debug, "readStructDecl");
	
	magic_num_require(MAGIC_STRUCTDECL, file);
	
	struct StructDecl* res = make(StructDecl);
	
	res->type = readSimpleType(file, debug);
	
	res->count_members = deserialize_int(file);
	
	res->members = malloc(sizeof(void*)*res->count_members);
	
	for(int i=0;i < res->count_members;i++){
		res->members[i] = readStructMember(file, debug);
	}
	
	magic_num_require(MAGIC_END_STRUCTDECL, file);
	
	return res;
}
struct StructMember* readStructMember(FILE* file, bool debug){
	
	status(debug, "readStructMember");
	
	magic_num_require(MAGIC_STRUCTMEMBER, file);
	
	struct StructMember* res = make(StructMember);
	
	res->type = readType(file, debug);
	
	char* tmp = deserialize_string(file);
	
	strcpy(res->name, tmp);
	free(tmp);
	
	magic_num_require(MAGIC_END_STRUCTMEMBER, file);
	
	return res;
}

struct StmtBlock* readStmtBlock(FILE* file, bool debug){
	
	status(debug, "readStmtBlock");
	
	magic_num_require(MAGIC_STMTBLOCK, file);
	
	struct StmtBlock* block = make(StmtBlock);
	
	block->count = deserialize_int(file);
	
	block->stmts = malloc(sizeof(void*)* block->count);
	
	for(int i = 0;i < block->count; i++){
		block->stmts[i] = readStmt(file, debug);
	}
	
	magic_num_require(MAGIC_END_STMTBLOCK, file);
	
	return block;
}

struct DeclArg* readDeclArg(FILE* file, bool debug){
	
	status(debug, "readDeclArg");
	
	magic_num_require(MAGIC_DECLARG, file);
	
	struct DeclArg* da = make(DeclArg);

	da->type = readType(file, debug);

	const int option = deserialize_int(file);

	da->has_name = option == OPT_PRESENT;

	if(option != 0 && option != 1){

		error(file, "Error in readDeclArg");
	}

	if(da->has_name){
		char* tmp = deserialize_string(file);
		strcpy(da->name, tmp);
		free(tmp);
	}
	
	magic_num_require(MAGIC_END_DECLARG, file);

	return da;
}
struct Expr* readExpr(FILE* file, bool debug){
	
	status(debug, "readExpr");
	
	magic_num_require(MAGIC_EXPR, file);
	
	struct Expr* expr = make(Expr);

	expr->term1 = readUnOpTerm(file, debug);
	expr->op    = NULL;
	expr->term2 = NULL;
	
	const int option = deserialize_int(file);
	
	if (option == OPT_PRESENT){
		
		expr->op    = readOp(file, debug);
		expr->term2 = readUnOpTerm(file, debug);
	}
	
	magic_num_require(MAGIC_END_EXPR, file);

	return expr;
}
struct Op* readOp(FILE* file, bool debug){
	
	status(debug, "readOp");
	
	magic_num_require(MAGIC_OP, file);
	
	struct Op* op = make(Op);
	
	fread(op, structsize(Op), 1, file);
	
	magic_num_require(MAGIC_END_OP, file);

	return op;
}
struct IntConst* readIntConst(FILE* file, bool debug){
	
	status(debug, "readIntConst");
	
	magic_num_require(MAGIC_INTCONST, file);
	
	struct IntConst* ic = make(IntConst);
		
	fread(ic, structsize(IntConst), 1, file);
	
	magic_num_require(MAGIC_END_INTCONST, file);

	return ic;
}

struct HexConst* readHexConst(FILE* file, bool debug){
	
	status(debug, "readHexConst");
	
	magic_num_require(MAGIC_HEXCONST, file);
	
	struct HexConst* hc = make(HexConst);
	
	fread(hc, structsize(HexConst), 1, file);
	
	magic_num_require(MAGIC_END_HEXCONST, file);

	return hc;
}

struct BinConst* readBinConst(FILE* file, bool debug){
	
	status(debug, "readBinConst");
	
	magic_num_require(MAGIC_BINCONST, file);
	
	struct BinConst* hc = make(BinConst);
	
	fread(hc, structsize(BinConst), 1, file);
	
	magic_num_require(MAGIC_END_BINCONST, file);

	return hc;
}

struct BoolConst* readBoolConst(FILE* file, bool debug){
	
	status(debug, "readBoolConst");
	
	magic_num_require(MAGIC_BOOLCONST, file);
	
	struct BoolConst* b = make(BoolConst);
	
	fread(b, structsize(BoolConst), 1, file);
	
	magic_num_require(MAGIC_END_BOOLCONST, file);
	
	return b;
}
struct CharConst* readCharConst(FILE* file, bool debug){
	
	status(debug, "readCharConst");
	
	magic_num_require(MAGIC_CHARCONST, file);
	
	struct CharConst* b = make(CharConst);
	
	fread(b, structsize(CharConst), 1, file);
	
	magic_num_require(MAGIC_END_CHARCONST, file);
	
	return b;
}
struct FloatConst* readFloatConst(FILE* file, bool debug){
	
	status(debug, "readFloatConst");
	
	magic_num_require(MAGIC_FLOATCONST, file);
	
	struct FloatConst* ic = make(FloatConst);
		
	fread(ic, structsize(FloatConst), 1, file);
	
	magic_num_require(MAGIC_END_FLOATCONST, file);
	
	return ic;
}
struct StringConst* readStringConst(FILE* file, bool debug){
	
	status(debug, "readStringConst");
	
	magic_num_require(MAGIC_STRINGCONST, file);
	
	struct StringConst* s = make(StringConst);
	
	//doing this to avoid problems
	//with whitespace or any characters at all really
	s->value = deserialize_string(file);
	
	magic_num_require(MAGIC_END_STRINGCONST, file);
	
	return s;
}
struct Variable* readVariable(FILE* file, bool debug){
	
	status(debug, "readVariable");
	
	magic_num_require(MAGIC_VARIABLE, file);
	
	struct Variable* v = make(Variable);
	v->memberAccess    = NULL;
	v->simpleVar       = readSimpleVar(file, debug);

	const bool hasMemberAccess = deserialize_int(file) == OPT_PRESENT;
	
	if(hasMemberAccess){
		v->memberAccess = readVariable(file, debug);
	}

	magic_num_require(MAGIC_END_VARIABLE, file);
	
	return v;
}
struct SimpleVar* readSimpleVar(FILE* file, bool debug){
	
	status(debug, "readSimpleVar");
	
	magic_num_require(MAGIC_SIMPLEVAR, file);
	
	struct SimpleVar* b = make(SimpleVar);
	
	char* tmp = deserialize_string(file);
	strcpy(b->name, tmp);
	free(tmp);

	b->count_indices = deserialize_int(file);
	
	b->indices = malloc(sizeof(void*)* (b->count_indices+1));
	
	for(int i=0; i < b->count_indices; i++){
		b->indices[i] = readExpr(file, debug);
	}
	
	magic_num_require(MAGIC_END_SIMPLEVAR, file);
	
	return b;
}
struct Term* readTerm(FILE* file, bool debug){
	
	status(debug, "readTerm");
	
	magic_num_require(MAGIC_TERM, file);
	
	struct Term* b = make(Term);
	
	b->kind = deserialize_int(file);

	switch(b->kind){
	
		case  1: b->ptr.m1  = readBoolConst(file, debug); 	break;
		case  2: b->ptr.m2  = readIntConst(file, debug); 	break;
		case  3: b->ptr.m3  = readCharConst(file, debug); 	break;
		case  4: b->ptr.m4  = readCall(file, debug); 	    break;
		case  5: b->ptr.m5  = readExpr(file, debug); 		break;
		case  6: b->ptr.m6  = readVariable(file, debug); 	break;
		case  7: b->ptr.m7  = readFloatConst(file, debug); 	break;
		case  8: b->ptr.m8  = readStringConst(file, debug); break;
		case  9: b->ptr.m9  = readHexConst(file, debug); 	break;
		case 10: b->ptr.m10 = readBinConst(file, debug); 	break;
		
		default:
			error(file, "Error in readTerm");
	}
	
	magic_num_require(MAGIC_END_TERM, file);
	
	return b;
}

struct UnOpTerm* readUnOpTerm(FILE* file, bool debug){
	
	status(debug, "readUnOpTerm");
	
	magic_num_require(MAGIC_UNOPTERM, file);
	
	const int opt      = deserialize_int(file);
	
	struct UnOpTerm* t = make(UnOpTerm);
	
	t->op   = (opt == OPT_PRESENT)? readOp(file, debug): NULL;
	t->term = readTerm(file, debug);
	
	magic_num_require(MAGIC_END_UNOPTERM, file);
	
	return t;
}

struct Range* readRange(FILE* file, bool debug){
	
	status(debug, "readRange");
	
	magic_num_require(MAGIC_RANGE, file);
	
	struct Range* r = make(Range);
	
	r->start = readExpr(file, debug);
	r->end   = readExpr(file, debug);
	
	magic_num_require(MAGIC_END_RANGE, file);
	
	return r;
}

//statementnodes
struct Stmt* readStmt(FILE* file, bool debug){
	
	status(debug, "readStmt");
	
	magic_num_require(MAGIC_STMT, file);
	
	struct Stmt* b = make(Stmt);
	
	b->kind = deserialize_int(file);

	switch(b->kind){
		case 99: 
			{
				b->isBreak    = deserialize_int(file) == OPT_PRESENT;
				b->isContinue = deserialize_int(file) == OPT_PRESENT;
			}
			break;
		case 0: b->ptr.m0 = readLoopStmt(file, debug);   break;
		case 1: b->ptr.m1 = readCall(file, debug);       break;
		case 2: b->ptr.m2 = readWhileStmt(file, debug);  break;
		case 3: b->ptr.m3 = readIfStmt(file, debug);     break;
		case 4: b->ptr.m4 = readRetStmt(file, debug);    break;
		case 5: b->ptr.m5 = readAssignStmt(file, debug); break;
		case 7: b->ptr.m7 = readForStmt(file, debug);  	 break;
		case 8: b->ptr.m8 = readSwitchStmt(file, debug); break;
		default:
			error(file, "Error in readStmt");
	}
	
	magic_num_require(MAGIC_END_STMT, file);
	
	return b;
}
struct IfStmt* readIfStmt(FILE* file, bool debug){
	
	status(debug, "readIfStmt");
	
	magic_num_require(MAGIC_IFSTMT, file);
	
	struct IfStmt* v = make(IfStmt);

	v->condition = readExpr(file, debug);
	v->block     = readStmtBlock(file, debug);
	v->elseBlock = NULL;

	const int hasElse = deserialize_int(file);
	
	if(hasElse == OPT_PRESENT){
		v->elseBlock = readStmtBlock(file, debug);
	}
	
	magic_num_require(MAGIC_END_IFSTMT, file);
	
	return v;
}
struct WhileStmt* readWhileStmt(FILE* file, bool debug){
	
	status(debug, "readWhileStmt");
	
	magic_num_require(MAGIC_WHILESTMT, file);
	
	struct WhileStmt* v = make(WhileStmt);

	v->condition = readExpr(file, debug);
	v->block     = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_WHILESTMT, file);
	
	return v;
}
struct RetStmt* readRetStmt(FILE* file, bool debug){
	
	status(debug, "readRetStmt");
	
	magic_num_require(MAGIC_RETSTMT, file);
	
	struct RetStmt* v = make(RetStmt);

	v->returnValue = readExpr(file, debug);
	
	magic_num_require(MAGIC_END_RETSTMT, file);
	
	return v;
}
struct AssignStmt* readAssignStmt(FILE* file, bool debug){
	
	status(debug, "readAssignStmt");
	
	magic_num_require(MAGIC_ASSIGNSTMT, file);
	
	const int option = deserialize_int(file);

	if(option != OPT_EMPTY && option != OPT_PRESENT){
		
		error(file, "Error in readAssignStmt\n");
	}
	
	struct AssignStmt* v = make(AssignStmt);

	v->optType = NULL;

	if(option == OPT_PRESENT){
		v->optType = readType(file, debug);
	}

	v->var = readVariable(file, debug);
	
	char* assign_op = deserialize_string(file);
	
	strncpy(v->assign_op, assign_op, ASSIGNOP_LENGTH);
	
	free(assign_op);
	
	v->expr = readExpr(file, debug);
	
	magic_num_require(MAGIC_END_ASSIGNSTMT, file);

	return v;
}
struct Call* readCall(FILE* file, bool debug){
	
	status(debug, "readCall");
	
	magic_num_require(MAGIC_CALL, file);
	
	struct Call* v = make(Call);

	char* tmp = deserialize_string(file);
	strcpy(v->name, tmp);
	free(tmp);
	
	v->count_args = deserialize_int(file);

	v->args = malloc(sizeof(void*)*(v->count_args));
	
	for(int i=0;i < (v->count_args);i++){
		v->args[i] = readExpr(file, debug);
	}
	
	magic_num_require(MAGIC_END_CALL, file);
	
	return v;
}
struct LoopStmt* readLoopStmt(FILE* file, bool debug){
	
	status(debug, "readLoopStmt");
	
	magic_num_require(MAGIC_LOOPSTMT, file);
	
	struct LoopStmt* v = make(LoopStmt);

	v->count = readExpr(file, debug);
	v->block = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_LOOPSTMT, file);
	
	return v;
}
struct ForStmt* readForStmt(FILE* file, bool debug){
	
	status(debug, "readForStmt");
	
	magic_num_require(MAGIC_FORSTMT, file);
	
	char* indexName = deserialize_string(file);
	
	struct ForStmt* res = make(ForStmt);
	
	strncpy(res->indexName, indexName, DEFAULT_STR_SIZE);
	free(indexName);
	
	res->range = readRange(file, debug);
	res->block = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_FORSTMT, file);
	
	return res;
}
struct SwitchStmt* readSwitchStmt(FILE* file, bool debug){
	
	status(debug, "readSwitchStmt");
	
	magic_num_require(MAGIC_SWITCHSTMT, file);
	
	struct SwitchStmt* res = make(SwitchStmt);
	
	res->expr = readExpr(file, debug);
	
	res->count_cases = deserialize_int(file);

	res->cases = malloc(sizeof(void*)* (res->count_cases));
	
	for(int i=0; i < res->count_cases; i++){
		
		res->cases[i] = readCaseStmt(file, debug);
	}
	
	magic_num_require(MAGIC_END_SWITCHSTMT, file);
	
	return res;
}
struct CaseStmt* readCaseStmt(FILE* file, bool debug){
	
	status(debug, "readCaseStmt");
	
	magic_num_require(MAGIC_CASESTMT, file);
	
	struct CaseStmt* res = make(CaseStmt);
	
	res->kind = deserialize_int(file);
	
	res->ptr.m1 = NULL;
	res->block  = NULL;
	
	switch(res->kind){
		case 0: res->ptr.m1 = readBoolConst(file, debug); break;
		case 1: res->ptr.m2 = readCharConst(file, debug); break;
		case 2: res->ptr.m3 = readIntConst(file, debug);  break;
		default:
			error(file, "Error in readCaseStmt");
	}
	
	const int hasBlock = deserialize_int(file);
	
	if(hasBlock == OPT_PRESENT){
		
		res->block = readStmtBlock(file, debug);
	}
	
	magic_num_require(MAGIC_END_CASESTMT, file);
	
	return res;
}
// --- typenodes -------------------------
struct Type* readType(FILE* file, bool debug){
	
	status(debug, "readType");
	
	magic_num_require(MAGIC_TYPE, file);
	
	const int kind = deserialize_int(file);;
	
	struct Type* b = make(Type);
	
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;

	switch(kind){
		case 1: b->m1 = readBasicTypeWrapped(file, debug); break;
		case 2: b->m2 = readTypeParam(file, debug);        break;
		case 3: b->m3 = readArrayType(file, debug);        break;
		default:
			error(file, "Error in readType");
	}
	
	magic_num_require(MAGIC_END_TYPE, file);
	
	return b;
}
struct SubrType* readSubrType(FILE* file, bool debug){
	
	status(debug, "readSubrType");
	
	magic_num_require(MAGIC_SUBRTYPE, file);
	
	struct SubrType* v = make(SubrType);
	
	v->returnType = readType(file, debug);
	
	v->hasSideEffects = deserialize_int(file);
	v->count_argTypes = deserialize_int(file);
	
	v->argTypes = malloc(sizeof(void*)*(v->count_argTypes));
	
	for(int i=0;i < (v->count_argTypes); i++){
		v->argTypes[i] = readType(file, debug);
	}
	
	magic_num_require(MAGIC_END_SUBRTYPE, file);
	
	return v;
}
struct SimpleType* readSimpleType(FILE* file, bool debug){
	
	status(debug, "readSimpleType");
	
	magic_num_require(MAGIC_SIMPLETYPE, file);
	
	struct SimpleType* v = make(SimpleType);
	
	v->primitiveType = NULL;
	v->structType    = NULL;
	
	const int kind = deserialize_int(file);
	
	switch(kind){
		
		case 0: v->primitiveType = readPrimitiveType(file, debug);
			break;
			
		case 1: v->structType = readStructType(file, debug);
			break;
			
		default:
			error(file, " kind not valid ");
	}
	
	magic_num_require(MAGIC_END_SIMPLETYPE, file);
	
	return v;
}
struct ArrayType* readArrayType(FILE* file, bool debug){
	
	status(debug, "readArrayType");
	
	magic_num_require(MAGIC_ARRAYTYPE, file);
	
	struct ArrayType* v = make(ArrayType);
	
	v->element_type = readType(file, debug);
	
	magic_num_require(MAGIC_END_ARRAYTYPE, file);
	
	return v;
}
struct TypeParam* readTypeParam(FILE* file, bool debug){
	
	status(debug, "readTypeParam");
	
	magic_num_require(MAGIC_TYPEPARAM, file);
	
	struct TypeParam* v = make(TypeParam);
		
	fread(v, structsize(TypeParam), 1, file);
	
	magic_num_require(MAGIC_END_TYPEPARAM, file);
	
	return v;
}
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file, bool debug){
	
	status(debug, "readBasicTypeWrapped");
	
	magic_num_require(MAGIC_BASICTYPEWRAPPED, file);
	
	struct BasicTypeWrapped* v = make(BasicTypeWrapped);

	v->simpleType = NULL;
	v->subrType   = NULL;
	
	const int kind = deserialize_int(file);
	
	switch(kind){

		case 1: v->simpleType = readSimpleType(file, debug); break;
		case 2: v->subrType   = readSubrType(file, debug);   break;

		default:
			error(file, "Error in readBasicTypeWrapped\n");
	}
	
	magic_num_require(MAGIC_END_BASICTYPEWRAPPED, file);
	
	return v;
}

struct StructType* readStructType(FILE* file, bool debug){
	
	status(debug, "readStructType");
	
	magic_num_require(MAGIC_STRUCTTYPE, file);
	
	struct StructType* res = make(StructType);
	
	char* tmp = deserialize_string(file);
	strcpy(res->typeName, tmp);
	free(tmp);
	
	res->typeParamCount = deserialize_int(file);
	
	if(res->typeParamCount > 0){
		res->typeParams = malloc(sizeof(uint8_t)*(res->typeParamCount));
	}
	
	for(int i = 0; i < res->typeParamCount; i++){
		
		res->typeParams[i] = deserialize_int(file);
	}

	magic_num_require(MAGIC_END_STRUCTTYPE, file);
	
	return res;
}

struct PrimitiveType* readPrimitiveType(FILE* file, bool debug){
	
	status(debug, "readPrimitiveType");
	
	magic_num_require(MAGIC_PRIMITIVETYPE, file);
	
	struct PrimitiveType* res = make(PrimitiveType);
	
	res->isIntType   = deserialize_int(file);
	res->isFloatType = deserialize_int(file);
	res->isCharType  = deserialize_int(file);
	res->isBoolType  = deserialize_int(file);
	
	res->intType     = deserialize_int(file);
	
	magic_num_require(MAGIC_END_PRIMITIVETYPE, file);
	
	return res;
}

