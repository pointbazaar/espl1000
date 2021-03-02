#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../ast.h"
#include "ast_reader.h"
#include "magic_num.h"
#include "serialize.h"

struct AST* readAST(char* filename, bool debug){
	//returns NULL if it is unable to open the file
	
	if(debug){ printf("readAST(...)\n"); }
	
	FILE* file = fopen(filename,"r");
	if(file == NULL){
		printf("could not open file: %s, in ast_reader.c:readAST\n", filename);
		return NULL;
	}

	struct AST* ast = malloc(sizeof(struct AST));

	magic_num_require(MAGIC_AST, file);

	struct Namespace* ns = readNamespace(file, debug);
	
	ast->count_namespaces = 1;
	ast->namespaces = malloc(sizeof(struct Namespace*));
	ast->namespaces[0] = ns;
	
	magic_num_require(MAGIC_END_AST, file);

	fclose(file);
	
	if(debug){ printf("readAST ~ done\n"); }
	
	return ast;
}

struct Namespace* readNamespace(FILE* file, bool debug){
	
	if(debug){ printf("readNamespace(...)\n"); }
	
	magic_num_require(MAGIC_NAMESPACE, file);
	
	struct Namespace* ns = malloc(sizeof(struct Namespace));

	char* tmpSrcPath = deserialize_string(file);
	strcpy(ns->srcPath, tmpSrcPath);
	free(tmpSrcPath);
	
	tmpSrcPath = deserialize_string(file);
	strcpy(ns->name, tmpSrcPath);
	free(tmpSrcPath);
	
	ns->count_methods = deserialize_int(file);

	//read methods
	ns->methods = malloc(sizeof(struct Method*)*(ns->count_methods));
	
	for(int i=0; i < ns->count_methods; i++){
		
		ns->methods[i] = readMethod(file, debug);
	}
	
	//read structs
	ns->count_structs = deserialize_int(file);
	
	ns->structs = 
		malloc(sizeof(struct StructDecl*)*(ns->count_structs));
	
	for(int i=0;i < ns->count_structs; i++){
		
		ns->structs[i] = readStructDecl(file, debug);
	}
	
	magic_num_require(MAGIC_END_NAMESPACE, file);
	
	return ns;
}
struct Method* readMethod(FILE* file, bool debug){
	
	if(debug){ printf("readMethod(...)\n"); }
	
	magic_num_require(MAGIC_METHOD, file);
	
	struct Method* m = malloc(sizeof(struct Method));

	m->isPublic = deserialize_int(file);
	m->hasSideEffects = deserialize_int(file);

	char* tmp = deserialize_string(file);
	strcpy(m->name, tmp);
	
	free(tmp);

	m->returnType = readType(file, debug);
	
	m->count_args = deserialize_int(file);
	
	m->args = malloc(sizeof(struct DeclArg*)*(m->count_args));

	for(int i = 0;i < m->count_args;i++){
		m->args[i] = readDeclArg(file, debug);
	}

	m->block = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_METHOD, file);
	
	return m;
}
struct StructDecl* readStructDecl(FILE* file, bool debug){
	
	if(debug){ printf("readStructDecl(...)\n"); }
	
	magic_num_require(MAGIC_STRUCTDECL, file);
	
	struct StructDecl* res = malloc(sizeof(struct StructDecl));
	
	res->type = readSimpleType(file, debug);
	
	res->count_members = deserialize_int(file);
	
	res->members = 
		malloc(sizeof(struct StructMember*)*res->count_members);
	for(int i=0;i < res->count_members;i++){
		res->members[i] = readStructMember(file, debug);
	}
	
	magic_num_require(MAGIC_END_STRUCTDECL, file);
	
	return res;
}
struct StructMember* readStructMember(FILE* file, bool debug){
	
	if(debug){ printf("readStructMember(...)\n"); }
	
	magic_num_require(MAGIC_STRUCTMEMBER, file);
	
	struct StructMember* res = 
		malloc(sizeof(struct StructMember));
	
	res->type = readType(file, debug);
	
	char* tmp = deserialize_string(file);
	
	strcpy(res->name, tmp);
	free(tmp);
	
	magic_num_require(MAGIC_END_STRUCTMEMBER, file);
	
	return res;
}

struct StmtBlock* readStmtBlock(FILE* file, bool debug){
	
	if(debug){ printf("readStmtBlock(...)\n"); }
	
	magic_num_require(MAGIC_STMTBLOCK, file);
	
	struct StmtBlock* block = malloc(sizeof(struct StmtBlock));
	
	block->count = deserialize_int(file);
	
	block->stmts = 
		malloc(sizeof(struct Stmt*)* block->count);
	
	for(int i=0;i < block->count; i++){
		block->stmts[i] = readStmt(file, debug);
	}
	
	magic_num_require(MAGIC_END_STMTBLOCK, file);
	
	return block;
}

// -----------------------

struct DeclArg* readDeclArg(FILE* file, bool debug){
	
	if(debug){ printf("readDeclArg(...)\n"); }
	
	magic_num_require(MAGIC_DECLARG, file);
	
	struct DeclArg* da = malloc(sizeof(struct DeclArg));

	da->type = readType(file, debug);

	int option = deserialize_int(file);

	da->has_name = option == OPT_PRESENT;

	if(option != 0 && option != 1){

		printf("Error in readDeclArg\n");
		free(da);
		fclose(file);
		exit(1);
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
	
	if(debug){ printf("readExpr(...)\n"); }
	
	magic_num_require(MAGIC_EXPR, file);
	
	struct Expr* expr = malloc(sizeof(struct Expr));

	expr->term1 = readUnOpTerm(file, debug);
	expr->op = NULL;
	expr->term2 = NULL;
	
	const int option = deserialize_int(file);

	if(option != OPT_EMPTY && option != OPT_PRESENT){
		printf("Error in readExpr!\n");
		fclose(file);
		exit(1);
	}
	
	if (option == OPT_PRESENT){
		expr->op = readOp(file, debug);
		expr->term2 = readUnOpTerm(file, debug);
	}
	
	magic_num_require(MAGIC_END_EXPR, file);

	return expr;
}
struct Op* readOp(FILE* file, bool debug){
	
	if(debug){ printf("readOp(...)\n"); }
	
	magic_num_require(MAGIC_OP, file);
	
	struct Op* op = malloc(sizeof(struct Op));
	
	fread(op, sizeof(struct Op), 1, file);
	
	magic_num_require(MAGIC_END_OP, file);

	return op;
}
struct IntConst* readIntConst(FILE* file, bool debug){
	
	if(debug){printf("readIntConst(...)\n");}
	
	magic_num_require(MAGIC_INTCONST, file);
	
	struct IntConst* ic = malloc(sizeof(struct IntConst));
		
	fread(ic, sizeof(struct IntConst), 1, file);
	
	magic_num_require(MAGIC_END_INTCONST, file);

	return ic;
}

struct HexConst* readHexConst(FILE* file, bool debug){
	
	if(debug){printf("readHexConst(...)\n");}
	
	magic_num_require(MAGIC_HEXCONST, file);
	
	struct HexConst* hc = malloc(sizeof(struct HexConst));
	
	fread(hc, sizeof(struct HexConst), 1, file);
	
	magic_num_require(MAGIC_END_HEXCONST, file);

	return hc;
}

struct BinConst* readBinConst(FILE* file, bool debug){
	
	if(debug){printf("readBinConst(...)\n");}
	
	magic_num_require(MAGIC_BINCONST, file);
	
	struct BinConst* hc = malloc(sizeof(struct BinConst));
	
	fread(hc, sizeof(struct BinConst), 1, file);
	
	magic_num_require(MAGIC_END_BINCONST, file);

	return hc;
}

struct BoolConst* readBoolConst(FILE* file, bool debug){
	
	if(debug){ printf("readBoolConst(...)\n"); }
	
	magic_num_require(MAGIC_BOOLCONST, file);
	
	struct BoolConst* b = malloc(sizeof(struct BoolConst));
	
	fread(b, sizeof(struct BoolConst), 1, file);
	
	magic_num_require(MAGIC_END_BOOLCONST, file);
	
	return b;
}
struct CharConst* readCharConst(FILE* file, bool debug){
	
	if(debug){ printf("readCharConst(...)\n"); }
	
	magic_num_require(MAGIC_CHARCONST, file);
	
	struct CharConst* b = malloc(sizeof(struct CharConst));
	
	fread(b, sizeof(struct CharConst), 1, file);
	
	magic_num_require(MAGIC_END_CHARCONST, file);
	
	return b;
}
struct FloatConst* readFloatConst(FILE* file, bool debug){
	
	if(debug){ printf("readFloatConst(...)\n"); }
	
	magic_num_require(MAGIC_FLOATCONST, file);
	
	struct FloatConst* ic = malloc(sizeof(struct FloatConst));
		
	fread(ic, sizeof(struct FloatConst), 1, file);
	
	magic_num_require(MAGIC_END_FLOATCONST, file);
	
	return ic;
}
struct StringConst* readStringConst(FILE* file, bool debug){
	
	if(debug){ printf("readStringConst(...)\n"); }
	
	magic_num_require(MAGIC_STRINGCONST, file);
	
	struct StringConst* s = malloc(sizeof(struct StringConst));
	
	//doing this to avoid problems
	//with whitespace or any characters at all really
	s->value = deserialize_string(file);
	
	magic_num_require(MAGIC_END_STRINGCONST, file);
	
	return s;
}
struct Variable* readVariable(FILE* file, bool debug){
	
	if(debug){ printf("readVariable(...)\n"); }
	
	magic_num_require(MAGIC_VARIABLE, file);
	
	struct Variable* v = malloc(sizeof(struct Variable));
	v->memberAccess = NULL;
	v->simpleVar = readSimpleVar(file, debug);

	const bool hasMemberAccess = deserialize_int(file) == OPT_PRESENT;
	
	if(hasMemberAccess){
		v->memberAccess = readVariable(file, debug);
	}

	magic_num_require(MAGIC_END_VARIABLE, file);
	
	return v;
}
struct SimpleVar* readSimpleVar(FILE* file, bool debug){
	
	if(debug){ printf("readSimpleVar(...)\n"); }
	
	magic_num_require(MAGIC_SIMPLEVAR, file);
	
	struct SimpleVar* b = malloc(sizeof(struct SimpleVar));
	
	char* tmp = deserialize_string(file);
	strcpy(b->name, tmp);
	free(tmp);

	b->count_indices = deserialize_int(file);
	
	b->indices = malloc(sizeof(struct Expr*)* (b->count_indices+1));
	
	for(int i=0; i < b->count_indices; i++){
		b->indices[i] = readExpr(file, debug);
	}
	
	magic_num_require(MAGIC_END_SIMPLEVAR, file);
	
	return b;
}
struct Term* readTerm(FILE* file, bool debug){
	
	if(debug){ printf("readTerm(...)\n"); }
	
	magic_num_require(MAGIC_TERM, file);
	
	struct Term* b = malloc(sizeof(struct Term));
	
	b->kind = deserialize_int(file);

	switch(b->kind){
	
		case  1: b->ptr.m1  = readBoolConst(file, debug); 	break;
		case  2: b->ptr.m2  = readIntConst(file, debug); 	break;
		case  3: b->ptr.m3  = readCharConst(file, debug); 	break;
		case  4: b->ptr.m4  = readMethodCall(file, debug); 	break;
		case  5: b->ptr.m5  = readExpr(file, debug); 		break;
		case  6: b->ptr.m6  = readVariable(file, debug); 	break;
		case  7: b->ptr.m7  = readFloatConst(file, debug); 	break;
		case  8: b->ptr.m8  = readStringConst(file, debug); break;
		case  9: b->ptr.m9  = readHexConst(file, debug); 	break;
		case 10: b->ptr.m10 = readBinConst(file, debug); 	break;
		
		default:
			printf("Error in readTerm\n");
			free(b);
			fclose(file);
			exit(1);
			break;
	}
	
	magic_num_require(MAGIC_END_TERM, file);
	
	return b;
}

struct UnOpTerm* readUnOpTerm(FILE* file, bool debug){
	
	if(debug){ printf("readUnOpTerm(...)\n"); }
	
	magic_num_require(MAGIC_UNOPTERM, file);
	
	const int opt = deserialize_int(file);
	
	struct UnOpTerm* t = malloc(sizeof(struct UnOpTerm));
	
	t->op = (opt == OPT_PRESENT)? readOp(file, debug): NULL;
	
	t->term = readTerm(file, debug);
	
	magic_num_require(MAGIC_END_UNOPTERM, file);
	
	return t;
}

struct Range* readRange(FILE* file, bool debug){
	
	if(debug){ printf("readRange(...)\n"); }
	
	magic_num_require(MAGIC_RANGE, file);
	
	struct Range* r = malloc(sizeof(struct Range));
	
	r->start = readExpr(file, debug);
	r->end = readExpr(file, debug);
	
	magic_num_require(MAGIC_END_RANGE, file);
	
	return r;
}

//statementnodes
struct Stmt* readStmt(FILE* file, bool debug){
	
	if(debug){ printf("readStmt(...)\n"); }
	
	magic_num_require(MAGIC_STMT, file);
	
	struct Stmt* b = malloc(sizeof(struct Stmt));
	
	b->kind = deserialize_int(file);

	switch(b->kind){
		case 0: b->ptr.m0 = readLoopStmt(file, debug);   break;
		case 1: b->ptr.m1 = readMethodCall(file, debug); break;
		case 2: b->ptr.m2 = readWhileStmt(file, debug);  break;
		case 3: b->ptr.m3 = readIfStmt(file, debug);     break;
		case 4: b->ptr.m4 = readRetStmt(file, debug);    break;
		case 5: b->ptr.m5 = readAssignStmt(file, debug); break;
		case 6: b->ptr.m6 = readBreakStmt(file, debug);  break;
		case 7: b->ptr.m7 = readForStmt(file, debug);  	 break;
		case 8: b->ptr.m8 = readSwitchStmt(file, debug); break;
		default:
			printf("Error in readStmt\n");
			free(b);
			fclose(file);
			exit(1);
			break;
	}
	
	magic_num_require(MAGIC_END_STMT, file);
	
	return b;
}
struct IfStmt* readIfStmt(FILE* file, bool debug){
	
	if(debug){ printf("readIfStmt(...)\n"); }
	
	magic_num_require(MAGIC_IFSTMT, file);
	
	struct IfStmt* v = malloc(sizeof(struct IfStmt));
	
	v->elseBlock = NULL;

	v->condition = readExpr(file, debug);

	v->block = readStmtBlock(file, debug);

	const int hasElse = deserialize_int(file);
	
	if(hasElse == OPT_PRESENT){
		v->elseBlock = readStmtBlock(file, debug);
	}
	
	magic_num_require(MAGIC_END_IFSTMT, file);
	
	return v;
}
struct WhileStmt* readWhileStmt(FILE* file, bool debug){
	
	if(debug){ printf("readWhileStmt(...)\n"); }
	
	magic_num_require(MAGIC_WHILESTMT, file);
	
	struct WhileStmt* v = malloc(sizeof(struct WhileStmt));

	v->condition = readExpr(file, debug);
	v->block = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_WHILESTMT, file);
	
	return v;
}
struct RetStmt* readRetStmt(FILE* file, bool debug){
	
	if(debug){ printf("readRetStmt(...)\n"); }
	
	magic_num_require(MAGIC_RETSTMT, file);
	
	struct RetStmt* v = malloc(sizeof(struct RetStmt));

	v->returnValue = readExpr(file, debug);
	
	magic_num_require(MAGIC_END_RETSTMT, file);
	
	return v;
}
struct AssignStmt* readAssignStmt(FILE* file, bool debug){
	
	if(debug){ printf("readAssignStmt(...)\n"); }
	
	magic_num_require(MAGIC_ASSIGNSTMT, file);
	
	const int option = deserialize_int(file);

	if(option != OPT_EMPTY && option != OPT_PRESENT){
		
		printf("Error in readAssignStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct AssignStmt* v = malloc(sizeof(struct AssignStmt));

	v->optType = NULL;

	if(option == OPT_PRESENT){
		v->optType = readType(file, debug);
	}

	v->var = readVariable(file, debug);
	
	char* assign_op = deserialize_string(file);
	
	strncpy(v->assign_op, assign_op, 2);
	
	free(assign_op);
	
	v->expr = readExpr(file, debug);
	
	magic_num_require(MAGIC_END_ASSIGNSTMT, file);

	return v;
}
struct MethodCall* readMethodCall(FILE* file, bool debug){
	
	if(debug){ printf("readMethodCall(...)\n"); }
	
	magic_num_require(MAGIC_METHODCALL, file);
	
	struct MethodCall* v = 
		malloc(sizeof(struct MethodCall));

	char* tmp = deserialize_string(file);
	strcpy(v->methodName, tmp);
	free(tmp);
	
	v->count_args = deserialize_int(file);

	v->args = 
		malloc(sizeof(struct Expr*)*(v->count_args));
	for(int i=0;i < (v->count_args);i++){
		v->args[i] = readExpr(file, debug);
	}
	
	magic_num_require(MAGIC_END_METHODCALL, file);
	
	return v;
}
struct LoopStmt* readLoopStmt(FILE* file, bool debug){
	
	if(debug){ printf("readLoopStmt(...)\n"); }
	
	magic_num_require(MAGIC_LOOPSTMT, file);
	
	struct LoopStmt* v = malloc(sizeof(struct LoopStmt));

	v->count = readExpr(file, debug);
	v->block = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_LOOPSTMT, file);
	
	return v;
}
struct BreakStmt* readBreakStmt(FILE* file, bool debug){
	
	if(debug){ printf("readBreakStmt(...)\n"); }
	
	magic_num_require(MAGIC_BREAKSTMT, file);
	
	struct BreakStmt* v = malloc(sizeof(struct BreakStmt));
	
	magic_num_require(MAGIC_END_BREAKSTMT, file);
	
	return v;
}
struct ForStmt* readForStmt(FILE* file, bool debug){
	
	if(debug){ printf("readForStmt(...)\n"); }
	
	magic_num_require(MAGIC_FORSTMT, file);
	
	char* indexName = deserialize_string(file);
	
	struct ForStmt* res = malloc(sizeof(struct ForStmt));
	
	strncpy(res->indexName, indexName, DEFAULT_STR_SIZE);
	free(indexName);
	
	res->range = readRange(file, debug);
	res->block = readStmtBlock(file, debug);
	
	magic_num_require(MAGIC_END_FORSTMT, file);
	
	return res;
}
struct SwitchStmt* readSwitchStmt(FILE* file, bool debug){
	
	if(debug){ printf("readSwitchStmt(...)\n"); }
	
	magic_num_require(MAGIC_SWITCHSTMT, file);
	
	struct SwitchStmt* res = malloc(sizeof(struct SwitchStmt));
	
	res->var = readVariable(file, debug);
	
	res->count_cases = deserialize_int(file);

	res->cases = malloc(sizeof(struct CaseStmt*)* (res->count_cases));
	
	for(int i=0; i < res->count_cases; i++){
		
		res->cases[i] = readCaseStmt(file, debug);
	}
	
	magic_num_require(MAGIC_END_SWITCHSTMT, file);
	
	return res;
}
struct CaseStmt* readCaseStmt(FILE* file, bool debug){
	
	if(debug){ printf("readCaseStmt(...)\n"); }
	
	magic_num_require(MAGIC_CASESTMT, file);
	
	struct CaseStmt* res = malloc(sizeof(struct CaseStmt));
	
	res->kind = deserialize_int(file);
	
	res->ptr.m1 = NULL;
	res->block = NULL;
	
	switch(res->kind){
		case 0: res->ptr.m1 = readBoolConst(file, debug); break;
		case 1: res->ptr.m2 = readCharConst(file, debug); break;
		case 2: res->ptr.m3 = readIntConst(file, debug); break;
		default:
			printf("Error in readCase\n");
			free(res);
			fclose(file);
			exit(1);
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
	
	if(debug){ printf("readType(...)\n"); }
	
	magic_num_require(MAGIC_TYPE, file);
	
	const int kind = deserialize_int(file);;
	
	struct Type* b = malloc(sizeof(struct Type));
	
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;

	switch(kind){
		case 1: b->m1 = readBasicTypeWrapped(file, debug); break;
		case 2: b->m2 = readTypeParam(file, debug); break;
		case 3: b->m3 = readArrayType(file, debug); break;
		default:
			printf("Error in readType\n");
			free(b);
			fclose(file);
			exit(1);
	}
	
	magic_num_require(MAGIC_END_TYPE, file);
	
	return b;
}
struct SubrType* readSubrType(FILE* file, bool debug){
	
	if(debug){ printf("readSubrType(...)\n"); }
	
	magic_num_require(MAGIC_SUBRTYPE, file);
	
	struct SubrType* v = 
		malloc(sizeof(struct SubrType));

	v->returnType = readType(file, debug);
	
	v->hasSideEffects = deserialize_int(file);
	v->count_argTypes = deserialize_int(file);
	
	v->argTypes = malloc(sizeof(struct Type*)*(v->count_argTypes));
	
	for(int i=0;i < (v->count_argTypes); i++){
		v->argTypes[i] = readType(file, debug);
	}
	
	magic_num_require(MAGIC_END_SUBRTYPE, file);
	
	return v;
}
struct SimpleType* readSimpleType(FILE* file, bool debug){
	
	if(debug){ printf("readSimpleType(...)\n"); }
	
	magic_num_require(MAGIC_SIMPLETYPE, file);
	
	struct SimpleType* v = malloc(sizeof(struct SimpleType));
	
	char* tmp = deserialize_string(file);
	strcpy(v->typeName, tmp);
	free(tmp);
	
	v->isPrimitive = deserialize_int(file);
	v->isIntType   = deserialize_int(file);
	
	v->typeParamCount = deserialize_int(file);
	
	if(v->typeParamCount > 0){
		v->typeParams = malloc(sizeof(uint8_t)*(v->typeParamCount));
	}
	
	for(int i=0;i<v->typeParamCount;i++){
		
		v->typeParams[i] = deserialize_int(file);
	}
	
	magic_num_require(MAGIC_END_SIMPLETYPE, file);
	
	return v;
}
struct ArrayType* readArrayType(FILE* file, bool debug){
	
	if(debug){ printf("readArrayType(...)\n"); }
	
	magic_num_require(MAGIC_ARRAYTYPE, file);
	
	struct ArrayType* v = malloc(sizeof(struct ArrayType));
	
	v->element_type = readType(file, debug);
	
	magic_num_require(MAGIC_END_ARRAYTYPE, file);
	
	return v;
}
struct TypeParam* readTypeParam(FILE* file, bool debug){
	
	if(debug){ printf("readTypeParam(...)\n"); }
	
	magic_num_require(MAGIC_TYPEPARAM, file);
	
	struct TypeParam* v = malloc(sizeof(struct TypeParam));
		
	fread(v, sizeof(struct TypeParam), 1, file);
	
	magic_num_require(MAGIC_END_TYPEPARAM, file);
	
	return v;
}
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file, bool debug){
	
	if(debug){ printf("readBasicTypeWrapped(...)\n"); }
	
	magic_num_require(MAGIC_BASICTYPEWRAPPED, file);
	
	const int kind = deserialize_int(file);
	
	struct BasicTypeWrapped* v = malloc(sizeof(struct BasicTypeWrapped));

	v->simpleType = NULL;
	v->subrType = NULL;
	
	switch(kind){

		case 1: v->simpleType = readSimpleType(file, debug); break;
		case 2: v->subrType   = readSubrType(file, debug);   break;

		default:
			printf("Error in readBasicTypeWrapped\n");
			free(v);
			fclose(file);
			exit(1);
	}
	
	magic_num_require(MAGIC_END_BASICTYPEWRAPPED, file);
	
	return v;
}

