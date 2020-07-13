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

	for(int i=0; i < ns->count_methods; i++){
		struct Method* m =  readMethod(file, debug);
		if(m == NULL){
			fclose(file);
			printf("error in readNamespace 2\n");
			exit(1);
		}
		ns->methods[i] = m;
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

	m->returnType = readType(file);
	if(m->returnType == NULL){return NULL;}

	fscanf(file,"%d\t",&(m->count_args));

	for(int i = 0;i < m->count_args;i++){
		m->args[i] = readDeclArg(file);
	}

	fscanf(file,"%d\t",&(m->count_stmts));

	for(int i = 0;i < m->count_stmts;i++){
		m->stmts[i] = readStmt(file);
	}
	return m;
}
struct DeclArg* readDeclArg(FILE* file){
	struct DeclArg* da = malloc(sizeof(struct DeclArg));

	fscanf(file, "DeclaredArg\t");

	da->type = readType(file);

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
struct Expr* readExpr(FILE* file){
	struct Expr* expr = malloc(sizeof(struct Expr));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"Expr") != 0){ return NULL; }

	expr->term1 = readTerm(file);
	fscanf(file, "%s\t",next);
	if(strcmp(next, "NULL")==0){
		expr->op = NULL;
		expr->term2 = NULL;
	}else{
		expr->op = readOp(file);
		expr->term2 = readTerm(file);
	}

	return expr;
}
struct Op* readOp(FILE* file){
	struct Op* op = malloc(sizeof(struct Op));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"Op") != 0){ return NULL; }

	fscanf(file, "%s\t", op->op);
	return op;
}
struct IntConst* readIntConst(FILE* file){
	struct IntConst* ic = malloc(sizeof(struct IntConst));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"IntConst") != 0){ return NULL; }

	fscanf(file, "%d\t", &(ic->value));
	return ic;
}
struct BoolConst* readBoolConst(FILE* file){
	struct BoolConst* b = malloc(sizeof(struct BoolConst));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"BoolConst") != 0){ return NULL; }

	fscanf(file, "%d\t", (int*)(b->value));
	return b;
}
struct CharConst* readCharConst(FILE* file){
	struct CharConst* b = malloc(sizeof(struct CharConst));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"CharConst") != 0){ return NULL; }

	fscanf(file, "%c\t", &(b->value));
	return b;
}
struct FloatConst* readFloatConst(FILE* file){
	struct FloatConst* ic = malloc(sizeof(struct FloatConst));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"FloatConst") != 0){ return NULL; }

	fscanf(file, "%f\t", &(ic->value));
	return ic;
}
struct Variable* readVariable(FILE* file){
	struct Variable* v = malloc(sizeof(struct Variable));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"Variable") != 0){ return NULL; }

	v->simpleVar = readSimpleVar(file);
	int memberAccessCount = 0;
	fscanf(file, "%d\t", &memberAccessCount);

	v->memberAccessList = malloc(sizeof(struct SimpleVar*)*memberAccessCount);
	for(int i=0;i<memberAccessCount;i++){
		v->memberAccessList[i] = readVariable(file);
	}
	return v;
}
struct SimpleVar* readSimpleVar(FILE* file){
	struct SimpleVar* b = malloc(sizeof(struct SimpleVar));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"SimpleVar") != 0){ return NULL; }

	fscanf(file, "%s\t", b->name);
	fscanf(file, "%s\t",next);

	if(strcmp(next, "NULL") != 0){
		b->indexOptional = readExpr(file);
	}else{
		b->indexOptional = NULL;
	}
	return b;
}
struct Term* readTerm(FILE* file){
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
		case 1: b->m1 = readBoolConst(file); break;
		case 2: b->m2 = readIntConst(file); break;
		case 3: b->m3 = readCharConst(file); break;
		case 4: b->m4 = readMethodCall(file); break;
		case 5: b->m5 = readExpr(file); break;
		case 6: b->m6 = readVariable(file); break;
		case 7: b->m7 = readFloatConst(file); break;
	}
	return b;
}

//statementnodes
struct Stmt* readStmt(FILE* file){
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
		case 1: b->m1 = readMethodCall(file); break;
		case 2: b->m2 = readWhileStmt(file); break;
		case 3: b->m3 = readIfStmt(file); break;
		case 4: b->m4 = readRetStmt(file); break;
		case 5: b->m5 = readAssignStmt(file); break;
	}
	return b;
}
struct IfStmt* readIfStmt(FILE* file){
	struct IfStmt* v = malloc(sizeof(struct IfStmt));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"IfStmt") != 0){ return NULL; }

	v->condition = readExpr(file);

	v->count_statements = 0;
	fscanf(file, "%d\t", &(v->count_statements));
	v->statements = malloc(sizeof(struct Stmt*)*(v->count_statements));
	for(int i=0;i < (v->count_statements);i++){
		v->statements[i] = readStmt(file);
	}

	v->count_elseStatements = 0;
	fscanf(file, "%d\t", &(v->count_elseStatements));
	v->elseStatements = malloc(sizeof(struct Stmt*)*(v->count_elseStatements));
	for(int i=0;i < (v->count_elseStatements);i++){
		v->elseStatements[i] = readStmt(file);
	}
	return v;
}
struct WhileStmt* readWhileStmt(FILE* file){
	struct WhileStmt* v = malloc(sizeof(struct WhileStmt));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"WhileStmt") != 0){ return NULL; }

	v->condition = readExpr(file);

	v->count_statements = 0;
	fscanf(file, "%d\t", &(v->count_statements));
	v->statements = malloc(sizeof(struct Stmt*)*(v->count_statements));
	for(int i=0;i < (v->count_statements);i++){
		v->statements[i] = readStmt(file);
	}
	return v;
}
struct RetStmt* readRetStmt(FILE* file){
	struct RetStmt* v = malloc(sizeof(struct RetStmt));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"RetStmt") != 0){ return NULL; }

	v->returnValue = readExpr(file);
	return v;
}
struct AssignStmt* readAssignStmt(FILE* file){
	struct AssignStmt* v = malloc(sizeof(struct AssignStmt));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"AssignStmt") != 0){ return NULL; }

	fscanf(file, "%s\t", next);
	if(strcmp(next, "NULL") == 0){
		v->optType = NULL;
	}else{
		v->optType = readType(file);
	}

	v->var = readVariable(file);
	v->expr = readExpr(file);

	return v;
}
struct MethodCall* readMethodCall(FILE* file){
	struct MethodCall* v = malloc(sizeof(struct MethodCall));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"MethodCall") != 0){ return NULL; }


	fscanf(file, "%s\t%d\t", v->methodName, &(v->count_args));

	v->args = malloc(sizeof(struct Expr*)*(v->count_args));
	for(int i=0;i < (v->count_args);i++){
		v->args[i] = readExpr(file);
	}
	return v;
}

//typenodes
struct Type* readType(FILE* file){
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
		case 1: b->m1 = readBasicTypeWrapped(file); break;
		case 2: b->m2 = readTypeParam(file); break;
		case 3: b->m3 = readArrayType(file); break;
	}
	return b;
}
struct SubrType* readSubrType(FILE* file){
	struct SubrType* v = malloc(sizeof(struct SubrType));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"SubrType") != 0){ return NULL; }

	v->returnType = readType(file);
	fscanf(file, "%d\t", (int*)(v->hasSideEffects));

	fscanf(file, "%d\t", &(v->count_argTypes));
	v->argTypes = malloc(sizeof(struct Type*)*(v->count_argTypes));
	for(int i=0;i < (v->count_argTypes); i++){
		v->argTypes[i] = readType(file);
	}
	return v;
}
struct SimpleType* readSimpleType(FILE* file){
	struct SimpleType* v = malloc(sizeof(struct SimpleType));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"SimpleType") != 0){ return NULL; }
	
	fscanf(file, "%s\t", v->typeName);
	return v;
}
struct ArrayType* readArrayType(FILE* file){
	struct ArrayType* v = malloc(sizeof(struct ArrayType));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"ArrayType") != 0){ return NULL; }
	v->element_type = readType(file);
	return v;
}
struct TypeParam* readTypeParam(FILE* file){
	struct TypeParam* v = malloc(sizeof(struct TypeParam));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"TypeParam") != 0){ return NULL; }
	fscanf(file, "%d\t", &(v->index));
	return v;
}
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file){
	struct BasicTypeWrapped* v = malloc(sizeof(struct BasicTypeWrapped));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"BasicTypeWrapped") != 0){ return NULL; }
	
	int kind = 0;
	fscanf(file, "%d\t", &kind);
	switch(kind){
		case 1: 
			v->m1 = readSimpleType(file);
			v->m2 = NULL;
			break;
		case 2: 
			v->m1 = NULL;
			v->m2 = readSubrType(file);
			break;
	}
	return v;
}

