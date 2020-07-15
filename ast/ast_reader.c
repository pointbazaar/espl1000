#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "ast_reader.h"

struct AST_Whole_Program* readAST(char* filename, bool debug){
	FILE* file = fopen(filename,"r");
	if(file == NULL){return NULL;}

	struct AST_Whole_Program* ast = malloc(sizeof(struct AST_Whole_Program));

	struct Namespace* ns = readNamespace(file, debug);
	if(ns == NULL){return NULL;}
	ast->count_namespaces = 1;

	ast->namespaces = malloc(sizeof(struct Namespace*));
	ast->namespaces[0] = ns;

	fclose(file);
	return ast;
}

struct Namespace* readNamespace(FILE* file, bool debug){
	if(debug){
			printf("readNamespace(...)\n");
	}
	struct Namespace* ns = malloc(sizeof(struct Namespace));

	int count = fscanf(file,"%s %s %d ", ns->srcPath, ns->name, &(ns->count_methods));

	if(count != 3){
		fclose(file);
		printf("error in readNamespace ,%d\n",count);
		exit(1);
	}

	//read methods
	ns->methods = malloc(sizeof(struct Method*)*(ns->count_methods));
	for(int i=0; i < ns->count_methods; i++){
		struct Method* m =  readMethod(file, debug);
		if(m == NULL){
			fclose(file);
			printf("error in readNamespace 2\n");
			exit(1);
		}
		ns->methods[i] = m;
	}
	
	//read structs
	count = fscanf(file, "%d\t", &(ns->count_structs));
	if(count != 1){
		fclose(file);
		printf("error in readNamespace 3 ,%d\n",count);
		exit(1);
	}
	ns->structs = malloc(sizeof(struct StructDecl*)*(ns->count_structs));
	for(int i=0;i < ns->count_structs; i++){
		struct StructDecl* s = readStructDecl(file, debug);
		if(s == NULL){
			fclose(file);
			printf("error in readNamespace 4\n");
			exit(1);
		}
		ns->structs[i] = s;
	}
	
	return ns;
}
struct Method* readMethod(FILE* file, bool debug){
	if(debug){
			printf("readMethod(...)\n");
	}
	fscanf(file, "Method\t");
	struct Method* m = malloc(sizeof(struct Method));

	fscanf(file,"%d\t%d\t%s\t",(int*)&(m->isPublic),(int*)&(m->hasSideEffects), m->name);

	m->returnType = readType(file, debug);
	if(m->returnType == NULL){return NULL;}

	fscanf(file,"%d\t",&(m->count_args));

	for(int i = 0;i < m->count_args;i++){
		m->args[i] = readDeclArg(file, debug);
	}

	fscanf(file,"%d\t",&(m->count_stmts));

	for(int i = 0;i < m->count_stmts;i++){
		m->stmts[i] = readStmt(file, debug);
	}
	return m;
}
struct StructDecl* readStructDecl(FILE* file, bool debug){
	struct StructDecl* res = malloc(sizeof(struct StructDecl));
	fscanf(file, "StructDecl\t");
	fscanf(file, "%s\t%d\t",res->name, &(res->count_members));
	res->members = malloc(sizeof(struct StructMember*)*res->count_members);
	for(int i=0;i < res->count_members;i++){
		res->members[i] = readStructMember(file, debug);
	}
	return res;
}
struct StructMember* readStructMember(FILE* file, bool debug){
	struct StructMember* res = malloc(sizeof(struct StructMember));
	fscanf(file, "StructMember\t");
	res->type = readType(file, debug);
	fscanf(file, "%s\t", res->name);
	return res;
}
struct DeclArg* readDeclArg(FILE* file, bool debug){
	struct DeclArg* da = malloc(sizeof(struct DeclArg));

	fscanf(file, "DeclaredArg\t");

	da->type = readType(file, debug);

	char next[10];
	fscanf(file, "%s\t", next);
	if(strcmp(next, "NULL")==0){
		da->has_name = false;
	}else{
		strcpy(da->name, next);
		da->has_name = true;
	}

	return da;
}
struct Expr* readExpr(FILE* file, bool debug){
	struct Expr* expr = malloc(sizeof(struct Expr));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"Expr") != 0){ return NULL; }

	expr->term1 = readTerm(file, debug);
	fscanf(file, "%s\t",next);
	if(strcmp(next, "NULL")==0){
		expr->op = NULL;
		expr->term2 = NULL;
	}else{
		expr->op = readOp(file, debug);
		expr->term2 = readTerm(file, debug);
	}

	return expr;
}
struct Op* readOp(FILE* file, bool debug){
	struct Op* op = malloc(sizeof(struct Op));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"Op") != 0){ return NULL; }

	fscanf(file, "%s\t", op->op);
	return op;
}
struct IntConst* readIntConst(FILE* file, bool debug){
	struct IntConst* ic = malloc(sizeof(struct IntConst));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"IntConst") != 0){ return NULL; }

	fscanf(file, "%d\t", &(ic->value));
	return ic;
}
struct BoolConst* readBoolConst(FILE* file, bool debug){
	struct BoolConst* b = malloc(sizeof(struct BoolConst));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"BoolConst") != 0){ return NULL; }

	fscanf(file, "%d\t", (int*)(b->value));
	return b;
}
struct CharConst* readCharConst(FILE* file, bool debug){
	struct CharConst* b = malloc(sizeof(struct CharConst));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"CharConst") != 0){ return NULL; }

	fscanf(file, "%c\t", &(b->value));
	return b;
}
struct FloatConst* readFloatConst(FILE* file, bool debug){
	struct FloatConst* ic = malloc(sizeof(struct FloatConst));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"FloatConst") != 0){ return NULL; }

	fscanf(file, "%f\t", &(ic->value));
	return ic;
}
struct Variable* readVariable(FILE* file, bool debug){
	struct Variable* v = malloc(sizeof(struct Variable));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"Variable") != 0){ return NULL; }

	v->simpleVar = readSimpleVar(file, debug);
	int memberAccessCount = 0;
	fscanf(file, "%d\t", &memberAccessCount);

	v->memberAccessList = malloc(sizeof(struct SimpleVar*)*memberAccessCount);
	for(int i=0;i<memberAccessCount;i++){
		v->memberAccessList[i] = readVariable(file, debug);
	}
	return v;
}
struct SimpleVar* readSimpleVar(FILE* file, bool debug){
	struct SimpleVar* b = malloc(sizeof(struct SimpleVar));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"SimpleVar") != 0){ return NULL; }

	fscanf(file, "%s\t", b->name);
	fscanf(file, "%s\t",next);

	if(strcmp(next, "NULL") != 0){
		b->optIndex = readExpr(file, debug);
	}else{
		b->optIndex = NULL;
	}
	return b;
}
struct Term* readTerm(FILE* file, bool debug){
	struct Term* b = malloc(sizeof(struct Term));
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;
	b->m4 = NULL;
	b->m5 = NULL;
	b->m6 = NULL;
	b->m7 = NULL;

	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"Term") != 0){ return NULL; }
	
	int kind;
	fscanf(file, "%d\t", &kind);
	kind = atoi(next);

	switch(kind){
		case 1: b->m1 = readBoolConst(file, debug); break;
		case 2: b->m2 = readIntConst(file, debug); break;
		case 3: b->m3 = readCharConst(file, debug); break;
		case 4: b->m4 = readMethodCall(file, debug); break;
		case 5: b->m5 = readExpr(file, debug); break;
		case 6: b->m6 = readVariable(file, debug); break;
		case 7: b->m7 = readFloatConst(file, debug); break;
	}
	return b;
}

//statementnodes
struct Stmt* readStmt(FILE* file, bool debug){
	struct Stmt* b = malloc(sizeof(struct Stmt));
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;
	b->m4 = NULL;
	b->m5 = NULL;

	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"Stmt") != 0){ return NULL; }
	
	int kind;
	fscanf(file, "%d\t", &kind);
	kind = atoi(next);

	switch(kind){
		case 1: b->m1 = readMethodCall(file, debug); break;
		case 2: b->m2 = readWhileStmt(file, debug); break;
		case 3: b->m3 = readIfStmt(file, debug); break;
		case 4: b->m4 = readRetStmt(file, debug); break;
		case 5: b->m5 = readAssignStmt(file, debug); break;
	}
	return b;
}
struct IfStmt* readIfStmt(FILE* file, bool debug){
	struct IfStmt* v = malloc(sizeof(struct IfStmt));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"IfStmt") != 0){ return NULL; }

	v->condition = readExpr(file, debug);

	v->count_statements = 0;
	fscanf(file, "%d\t", &(v->count_statements));
	v->statements = malloc(sizeof(struct Stmt*)*(v->count_statements));
	for(int i=0;i < (v->count_statements);i++){
		v->statements[i] = readStmt(file, debug);
	}

	v->count_elseStatements = 0;
	fscanf(file, "%d\t", &(v->count_elseStatements));
	v->elseStatements = malloc(sizeof(struct Stmt*)*(v->count_elseStatements));
	for(int i=0;i < (v->count_elseStatements);i++){
		v->elseStatements[i] = readStmt(file, debug);
	}
	return v;
}
struct WhileStmt* readWhileStmt(FILE* file, bool debug){
	struct WhileStmt* v = malloc(sizeof(struct WhileStmt));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"WhileStmt") != 0){ return NULL; }

	v->condition = readExpr(file, debug);

	v->count_statements = 0;
	fscanf(file, "%d\t", &(v->count_statements));
	v->statements = malloc(sizeof(struct Stmt*)*(v->count_statements));
	for(int i=0;i < (v->count_statements);i++){
		v->statements[i] = readStmt(file, debug);
	}
	return v;
}
struct RetStmt* readRetStmt(FILE* file, bool debug){
	struct RetStmt* v = malloc(sizeof(struct RetStmt));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"RetStmt") != 0){ return NULL; }

	v->returnValue = readExpr(file, debug);
	return v;
}
struct AssignStmt* readAssignStmt(FILE* file, bool debug){
	struct AssignStmt* v = malloc(sizeof(struct AssignStmt));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"AssignStmt") != 0){ return NULL; }

	fscanf(file, "%s\t", next);
	if(strcmp(next, "NULL") == 0){
		v->optType = NULL;
	}else{
		v->optType = readType(file, debug);
	}

	v->var = readVariable(file, debug);
	v->expr = readExpr(file, debug);

	return v;
}
struct MethodCall* readMethodCall(FILE* file, bool debug){
	struct MethodCall* v = malloc(sizeof(struct MethodCall));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"MethodCall") != 0){ return NULL; }


	fscanf(file, "%s\t%d\t", v->methodName, &(v->count_args));

	v->args = malloc(sizeof(struct Expr*)*(v->count_args));
	for(int i=0;i < (v->count_args);i++){
		v->args[i] = readExpr(file, debug);
	}
	return v;
}

//typenodes
struct Type* readType(FILE* file, bool debug){
	if(debug){
			printf("readType(...)\n");
	}
	struct Type* b = malloc(sizeof(struct Type));
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;

	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"Type") != 0){ return NULL; }
	
	int kind;
	fscanf(file, "%d\t", &kind);
	kind = atoi(next);

	switch(kind){
		case 1: b->m1 = readBasicTypeWrapped(file, debug); break;
		case 2: b->m2 = readTypeParam(file, debug); break;
		case 3: b->m3 = readArrayType(file, debug); break;
	}
	return b;
}
struct SubrType* readSubrType(FILE* file, bool debug){
	struct SubrType* v = malloc(sizeof(struct SubrType));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"SubrType") != 0){ return NULL; }

	v->returnType = readType(file, debug);
	fscanf(file, "%d\t", (int*)(v->hasSideEffects));

	fscanf(file, "%d\t", &(v->count_argTypes));
	v->argTypes = malloc(sizeof(struct Type*)*(v->count_argTypes));
	for(int i=0;i < (v->count_argTypes); i++){
		v->argTypes[i] = readType(file, debug);
	}
	return v;
}
struct SimpleType* readSimpleType(FILE* file, bool debug){
	struct SimpleType* v = malloc(sizeof(struct SimpleType));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"SimpleType") != 0){ return NULL; }
	
	fscanf(file, "%s\t", v->typeName);
	return v;
}
struct ArrayType* readArrayType(FILE* file, bool debug){
	struct ArrayType* v = malloc(sizeof(struct ArrayType));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"ArrayType") != 0){ return NULL; }
	v->element_type = readType(file, debug);
	return v;
}
struct TypeParam* readTypeParam(FILE* file, bool debug){
	struct TypeParam* v = malloc(sizeof(struct TypeParam));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"TypeParam") != 0){ return NULL; }
	fscanf(file, "%d\t", &(v->index));
	return v;
}
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file, bool debug){
	struct BasicTypeWrapped* v = malloc(sizeof(struct BasicTypeWrapped));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"BasicTypeWrapped") != 0){ return NULL; }
	
	int kind = 0;
	fscanf(file, "%d\t", &kind);
	switch(kind){
		case 1: 
			v->simpleType = readSimpleType(file, debug);
			v->subrType = NULL;
			break;
		case 2: 
			v->simpleType = NULL;
			v->subrType = readSubrType(file, debug);
			break;
	}
	return v;
}

