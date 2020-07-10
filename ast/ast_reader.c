#include <stdlib.h>
#include <stdio.h>

#include "ast.h"


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

	fscanf(file,"%s\t%s\t%d\t",&(ns->srcPath),&(ns->name),&(ns->count_methods));

	for(int i=0; i < ns->count_methods; i++){
		struct Method* m =  readMethod(file);
		if(m == NULL){ return NULL; }
		ns->methods[i] = m;
	}
	return ns;
}
struct Method* readMethod(FILE* file){
	fscanf(file, "Method\t");

	fscanf(file,"%d\t%d\t%s\t",&(m->isPublic),&(m->hasSideEffects),&(m->methodName));

	struct Method* m = malloc(sizeof(struct Method));

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
	fscanf("%s\t",&next);
	if(strcmp(next, "NULL")==0){
		da->name = NULL;
	}else{
		strcpy(da->name, next);
	}

	return da;
}
struct Expr* readExpr(FILE* file);
struct Op* readOp(FILE* file);
struct IntConst* readIntConst(FILE* file);
struct BoolConst* readBoolConst(FILE* file);
struct CharConst* readCharConst(FILE* file);
struct FloatConst* readFloatConst(FILE* file);
struct Variable* readVariable(FILE* file);
struct SimpleVar* readSimpleVar(FILE* file);
struct Term* readTerm(FILE* file);

//statementnodes
struct Stmt* readStmt(FILE* file);
struct IfStmt* readIfStmt(FILE* file);
struct WhileStmt* readWhileStmt(FILE* file);
struct RetStmt* readRetStmt(FILE* file);
struct AssignStmt* readAssignStmt(FILE* file);
struct MethodCall* readMethodCall(FILE* file);

//typenodes
struct Type* readType(FILE* file);
struct SubrType* readSubrType(FILE* file);
struct SimpleType* readSimpleType(FILE* file);
struct ArrayType* readArrayType(FILE* file);
struct TypeParam* readTypeParam(FILE* file);
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file);

