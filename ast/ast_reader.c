#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "ast_reader.h"

struct AST_Whole_Program* readAST(char* filename){
	FILE* file = fopen(filename,"r");

	struct AST_Whole_Program* ast = malloc(sizeof(struct AST_Whole_Program));

	struct Namespace* ns = readNamespace(file);
	ast->count_namespaces = 1;

	ast->namespaces = malloc(sizeof(struct Namespace*));
	ast->namespaces[0] = ns;

	fclose(file);
	return ast;
}

struct Namespace* readNamespace(FILE* file){
	struct Namespace* ns = malloc(sizeof(struct Namespace));

	fscanf(file,"%s\t%s\t%d\t", ns->srcPath, ns->name,&(ns->count_methods));

	for(int i=0; i < ns->count_methods; i++){
		struct Method* m =  readMethod(file);
		if(m == NULL){ return NULL; }
		ns->methods[i] = m;
	}
	return ns;
}
struct Method* readMethod(FILE* file){
	fscanf(file, "Method\t");
	struct Method* m = malloc(sizeof(struct Method));

	fscanf(file,"%d\t%d\t%s\t",(int*)&(m->isPublic),(int*)&(m->hasSideEffects), m->methodName);

	m->returnType = readType(file);
	if(m->returnType == NULL){return NULL;}

	fscanf(file,"%d\t",&(m->count_arguments));

	for(int i = 0;i < m->count_arguments;i++){
		m->arguments[i] = readDeclArg(file);
	}

	fscanf(file,"%d\t",&(m->count_statements));

	for(int i = 0;i < m->count_statements;i++){
		m->statements[i] = readStmt(file);
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
	if(strcmp(next,"Expr") == 0){ return NULL; }

	return expr;
}
struct Op* readOp(FILE* file){
	//TODO
	return NULL;
}
struct IntConst* readIntConst(FILE* file){
	//TODO
	return NULL;
}
struct BoolConst* readBoolConst(FILE* file){
	//TODO
	return NULL;
}
struct CharConst* readCharConst(FILE* file){
	//TODO
	return NULL;
}
struct FloatConst* readFloatConst(FILE* file){
	//TODO
	return NULL;
}
struct Variable* readVariable(FILE* file){
	//TODO
	return NULL;
}
struct SimpleVar* readSimpleVar(FILE* file){
	//TODO
	return NULL;
}
struct Term* readTerm(FILE* file){
	//TODO
	return NULL;
}

//statementnodes
struct Stmt* readStmt(FILE* file){
	//TODO
	return NULL;
}
struct IfStmt* readIfStmt(FILE* file){
	//TODO
	return NULL;
}
struct WhileStmt* readWhileStmt(FILE* file){
	//TODO
	return NULL;
}
struct RetStmt* readRetStmt(FILE* file){
	//TODO
	return NULL;
}
struct AssignStmt* readAssignStmt(FILE* file){
	//TODO
	return NULL;
}
struct MethodCall* readMethodCall(FILE* file){
	//TODO
	return NULL;
}

//typenodes
struct Type* readType(FILE* file){
	//TODO
	return NULL;
}
struct SubrType* readSubrType(FILE* file){
	//TODO
	return NULL;
}
struct SimpleType* readSimpleType(FILE* file){
	//TODO
	return NULL;
}
struct ArrayType* readArrayType(FILE* file){
	//TODO
	return NULL;
}
struct TypeParam* readTypeParam(FILE* file){
	//TODO
	return NULL;
}
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file){
	//TODO
	return NULL;
}

