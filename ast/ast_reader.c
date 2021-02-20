#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "ast_reader.h"
#include "magic_num.h"

// --- private deserialization functions ---
int deserialize_int(FILE* file);
int deserialize_int(FILE* file){
	int res;
	fread(&res, sizeof(int), 1, file);
	return res;
}

void magic_num_require(uint32_t expected, FILE* file);
void magic_num_require(uint32_t expected, FILE* file){
	//paired with magic_num_serialize in ASTWriter.h
	//this routine requires to read a magic number from 'file'
	//in order to early-detect a corrupted .ast file
	const int actual = deserialize_int(file);
	if(actual != expected){
		printf("magic_num_require failed.\n");
		printf("required was: %d\n", expected);
		fclose(file);
		exit(1);
	}
}
// -----------------------------------------

struct AST_Whole_Program* readAST(char* filename, bool debug){
	//returns NULL if it is unable to open the file
	
	if(debug){ printf("readAST(...)\n"); }
	
	FILE* file = fopen(filename,"r");
	if(file == NULL){
		printf("could not open file: %s, in ast_reader.c:readAST\n", filename);
		return NULL;
	}

	struct AST_Whole_Program* ast = malloc(sizeof(struct AST_Whole_Program));

	magic_num_require(MAGIC_AST_WHOLE_PROGRAM, file);

	struct Namespace* ns = readNamespace(file, debug);
	
	ast->count_namespaces = 1;
	ast->namespaces = 
		malloc(sizeof(struct Namespace*));
	ast->namespaces[0] = ns;
	
	magic_num_require(MAGIC_END_AST_WHOLE_PROGRAM, file);

	fclose(file);
	
	if(debug){ printf("readAST ~ done\n"); }
	
	return ast;
}

struct Namespace* readNamespace(FILE* file, bool debug){
	
	if(debug){ printf("readNamespace(...)\n"); }
	
	magic_num_require(MAGIC_NAMESPACE, file);
	
	struct Namespace* ns = malloc(sizeof(struct Namespace));

	int count = fscanf(file,
		"%s\t%s\t", 
		ns->srcPath, 
		ns->name
	);

	if(count != 2){
		fclose(file);
		printf("error in readNamespace ,%d\n",count);
		free(ns);
		exit(1);
	}
	
	ns->count_methods = deserialize_int(file);

	//read methods
	ns->methods = malloc(sizeof(struct Method*)*(ns->count_methods));
	
	
	for(int i=0; i < ns->count_methods; i++){
		
		struct Method* m =  readMethod(file, debug);
		ns->methods[i] = m;
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

	if(fscanf(file, "%s\t", m->name) != 1){
		printf("Error reading Method \n");
		fclose(file);
		free(m);
		exit(1);
	}

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
	
	int count_members = 0;
	
	if(
		fscanf(
			file, "%d\t", 
			&(count_members)
		) != 1
	){
		printf("Error reading StructDecl\n");
		fclose(file);
		free(res);
		exit(1);
	}
	
	res->count_members = count_members;
	
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
	
	if(fscanf(file, "%s\t", res->name) != 1){
		printf("Error reading StructMember2\n");
		//freeType(res->type) 
		//but this subroutine is not included here
		fclose(file);
		free(res);
		exit(1);
	}
	
	magic_num_require(MAGIC_END_STRUCTMEMBER, file);
	
	return res;
}

struct StmtBlock* readStmtBlock(FILE* file, bool debug){
	
	if(debug){ printf("readStmtBlock(...)\n"); }
	
	magic_num_require(MAGIC_STMTBLOCK, file);
	
	struct StmtBlock* block = malloc(sizeof(struct StmtBlock));
	
	if(fscanf(file, "%u\t", (unsigned int*)&(block->count)) != 1){
		printf("Error reading StmtBlock\n");
		fclose(file);
		free(block);
		exit(1);
	}
	
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

	int option;
	if(fscanf(file, "%d\t", &option) != 1){
		printf("Error reading DeclaredArg 2\n");
		fclose(file);
		free(da);
		exit(1);
	}
	
	if(option == 0){
		da->has_name = false;
	}else if(option == 1){
		da->has_name = true;
		
		//do not read more than 19 chars
		if(fscanf(file, "%19s\t", da->name) != 1){
			printf("Error reading DeclaredArg 3\n");
			fclose(file);
			exit(1);
		}
	}else{
		printf("Error in readDeclArg\n");
		free(da);
		fclose(file);
		exit(1);
	}
	
	magic_num_require(MAGIC_END_DECLARG, file);

	return da;
}
struct Expr* readExpr(FILE* file, bool debug){
	
	if(debug){ printf("readExpr(...)\n"); }
	
	magic_num_require(MAGIC_EXPR, file);
	
	struct Expr* expr = malloc(sizeof(struct Expr));

	expr->term1 = readUnOpTerm(file, debug);
	
	const int option = deserialize_int(file);
	
	if(option == 1){
		expr->op = NULL;
		expr->term2 = NULL;
	}else if (option == 0){
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
	
	const int length = deserialize_int(file);
	
	struct StringConst* s = malloc(sizeof(struct StringConst));
	
	//doing this to avoid problems
	//with whitespace or any characters at all really
	char* val = malloc(sizeof(char)*(length+1));
	val[length]='\0';
	
	for(int i=0;i < length; i++){
		//0-padded on the left, 3 chars wide,
		//casted to unsigned integer
		int v;
		fscanf(file, "%03d_", &v);
		val[i]=v;
	}
	s->value = val;
	
	magic_num_require(MAGIC_END_STRINGCONST, file);
	
	return s;
}
struct Variable* readVariable(FILE* file, bool debug){
	
	if(debug){ printf("readVariable(...)\n"); }
	
	magic_num_require(MAGIC_VARIABLE, file);
	
	struct Variable* v = 
		malloc(sizeof(struct Variable));

	v->simpleVar = readSimpleVar(file, debug);
	
	int count = 0;
	if(fscanf(file, "%d\t", &count) != 1){
		printf("Error reading Variable 2\n");
		free(v);
		fclose(file);
		exit(1);
	}

	v->memberAccessList = 
		malloc(sizeof(struct SimpleVar*)  * count);
	for(int i = 0;i < count; i++){
		v->memberAccessList[i] = readVariable(file, debug);
	}
	
	v->count_memberAccessList = count;
	
	magic_num_require(MAGIC_END_VARIABLE, file);
	
	return v;
}
struct SimpleVar* readSimpleVar(FILE* file, bool debug){
	
	if(debug){ printf("readSimpleVar(...)\n"); }
	
	magic_num_require(MAGIC_SIMPLEVAR, file);
	
	struct SimpleVar* b = malloc(sizeof(struct SimpleVar));
	
	if(fscanf(file, "%s\t", b->name) != 1){
		printf("Error reading SimpleVar\n");
		free(b);
		fclose(file);
		exit(1);
	}

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
	
	const int kind = deserialize_int(file);
	
	struct UnOpTerm* t = malloc(sizeof(struct UnOpTerm));
	
	t->op = (kind == 1)? readOp(file, debug): NULL;
	
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
	
	v->condition = NULL;
	v->block = NULL;
	v->elseBlock = NULL;

	v->condition = readExpr(file, debug);

	v->block = readStmtBlock(file, debug);

	int hasElse=0;
	fscanf(file, "%d\t", &hasElse);
	if(hasElse != 0){
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
	
	int option;
	
	if(fscanf(file, "%d\t", &option) != 1){
		printf("Error reading AssignStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct AssignStmt* v = malloc(sizeof(struct AssignStmt));

	if(option == 0){
		v->optType = NULL;
	}else if(option ==1){
		v->optType = readType(file, debug);
	}

	v->var = readVariable(file, debug);
	
	char assign_op[3];
	if(fscanf(file, "%s\t", assign_op) != 1){
		fclose(file);
		printf("Error in readAssignStmt\n");
		exit(1);
	}
	v->assign_op[2]='\0';
	strncpy(v->assign_op, assign_op, 2);
	
	v->expr = readExpr(file, debug);
	
	magic_num_require(MAGIC_END_ASSIGNSTMT, file);

	return v;
}
struct MethodCall* readMethodCall(FILE* file, bool debug){
	
	if(debug){ printf("readMethodCall(...)\n"); }
	
	magic_num_require(MAGIC_METHODCALL, file);
	
	struct MethodCall* v = 
		malloc(sizeof(struct MethodCall));

	if(fscanf(file, "%s\t", v->methodName) != 1){
		printf("Error reading MethodCall\n");
		free(v);
		fclose(file);
		exit(1);
	}
	
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
	
	char indexName[DEFAULT_STR_SIZE];
	
	if(fscanf(file, "%s\t", indexName) != 1){
		printf("Error reading ForStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct ForStmt* res = malloc(sizeof(struct ForStmt));
	
	strncpy(res->indexName, indexName, DEFAULT_STR_SIZE);
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
	
	int hasBlock = 0;
	
	fscanf(file, "%d\t", &hasBlock);
	
	if(hasBlock == 1){
		
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
	
	if(
		fscanf(file, "%d\t%u\t", (int*)(&(v->hasSideEffects)), (unsigned int*)&(v->count_argTypes))
		!= 2
	){
		printf("Error reading SubrType 2\n");
		free(v);
		fclose(file);
		exit(1);
	}
	
	v->argTypes = 
		malloc(sizeof(struct Type*)*(v->count_argTypes));
	for(int i=0;i < (v->count_argTypes); i++){
		v->argTypes[i] = readType(file, debug);
	}
	
	magic_num_require(MAGIC_END_SUBRTYPE, file);
	
	return v;
}
struct SimpleType* readSimpleType(FILE* file, bool debug){
	
	if(debug){ printf("readSimpleType(...)\n"); }
	
	magic_num_require(MAGIC_SIMPLETYPE, file);
	
	struct SimpleType* v = 
		malloc(sizeof(struct SimpleType));
	
	//%30s ensures it does not read more than 30 chars
	if(fscanf(file, "%30s\t", v->typeName) != 1){
		printf("Error reading SimpleType\n");
		free(v);
		fclose(file);
		exit(1);
	}
	
	int count=0;
	fscanf(file, "%d\t", &count);
	v->typeParamCount = count;
	if(v->typeParamCount > 0){
		v->typeParams = 
			malloc(sizeof(uint8_t)*(v->typeParamCount));
	}
	for(int i=0;i<v->typeParamCount;i++){
		int pIndex;
		fscanf(file, "%d\t", &pIndex);
		v->typeParams[i] = pIndex;
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
	
	switch(kind){
		case 1: 
			v->simpleType = readSimpleType(file, debug);
			v->subrType = NULL;
			break;
		case 2: 
			v->simpleType = NULL;
			v->subrType = readSubrType(file, debug);
			break;
		default:
			printf("Error in readBasicTypeWrapped\n");
			free(v);
			fclose(file);
			exit(1);
	}
	
	magic_num_require(MAGIC_END_BASICTYPEWRAPPED, file);
	
	return v;
}

