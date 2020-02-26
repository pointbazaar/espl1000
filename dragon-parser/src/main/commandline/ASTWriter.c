#include <stdio.h>
#include <stdlib.h>

#include "ASTWriter.h"

void write(struct Namespace* nsn, FILE* file){
	fprintf(file,"%s\t%s\t%d\t",nsn->srcPath,nsn->name,nsn->count_methods);
	
	for(int i=0;i < nsn->count_methods;i++){ 
		struct Method* m = nsn->methods[i];
		write(m,file);
	}
}
void write(struct Method* m, FILE* file){
	fprintf(file,"%d\t%d\t%s\t",m->isPublic,m->hasSideEffects,m->methodName);
	write(m->returnType,file);

	fprintf(file,"%d\t",m->count_arguments);
	for(int i = 0;i < m->count_arguments;i++){ 
		struct DeclArg* arg = m->arguments[i];
		write(arg,file); 
	}

	fprintf(file,"%d\t",m->count_statements);
	for(int i = 0;i < m->count_statements;i++){ 
		struct Stmt* s = m->statements[i];
		write(s,file); 
	}
}
void write(struct DeclArg* m, FILE* file){
	write(m->type,file);
	if(m->name != NULL){
		fprintf(file,"%s\t",m->name);
	}else{
		fprintf(file,"NULL\t");
	}
}
void write(struct Variable* m, FILE* file){
	write(m->simpleVariableNode,file);
	for(int i = 0;i < m->count_memberAccessList;i++){ 
		struct Variable* v = m->memberAccessList[i];
		write(v,file); 
	}
}
void write(struct SimpleVar* m, FILE* file){
	fprintf(file,"%s\t",m->name);
	if(m->indexOptional != NULL){
		write(m->indexOptional,file);
	}else{
		fprintf(file,"NULL\t");
	}
}
void write(struct Expr* m, FILE* file){
	write(m->term1,file);
	if(m->op != NULL){
		write(m->op,file);
		write(m->term2,file);
	}else{
		fprintf(file,"NULL\t");
	}
}
void write(struct Term* m, FILE* file){
	if(m->m1 != NULL){ fprintf(file,"1\t"); write(m->m1,file); }
	if(m->m2 != NULL){ fprintf(file,"2\t"); write(m->m2,file); }
	if(m->m3 != NULL){ fprintf(file,"3\t"); write(m->m3,file); }
	if(m->m4 != NULL){ fprintf(file,"4\t"); write(m->m4,file); }
	if(m->m5 != NULL){ fprintf(file,"5\t"); write(m->m5,file); }
	if(m->m6 != NULL){ fprintf(file,"6\t"); write(m->m6,file); }
}
void write(struct BoolConst* m, FILE* file){
	fprintf(file,"%d\t",m->value);
}
void write(struct IntConst* m, FILE* file){
	fprintf(file,"%d\t",m->value);
}
void write(struct CharConst* m, FILE* file){
	fprintf(file,"%s\t",m->content);
}
void write(struct Op* m, FILE* file){
	fprintf(file,,"%s\t",m->op);
}
// ---------------- STATEMENTNODES ---------------------
void write(struct Stmt* m, FILE* file){
	//the reader has to know which type it is,
	//we can print a small number
	if(m->m1 != NULL){ fprintf(file,"1\t"); write(m->m1,file); }
	if(m->m2 != NULL){ fprintf(file,"2\t"); write(m->m2,file); }
	if(m->m3 != NULL){ fprintf(file,"3\t"); write(m->m3,file); }
	if(m->m4 != NULL){ fprintf(file,"4\t"); write(m->m4,file); }
	if(m->m5 != NULL){ fprintf(file,"5\t"); write(m->m5,file); }
}

void write(struct IfStmt* m, FILE* file){
	write(m->condition,file);
	for(int i = 0;i < m->count_statements;i++){ 
		struct Stmt* s = m->statements[i];
		write(s,file); 
	}

	for(int i = 0;i < m->count_elseStatements;i++){ 
		struct Stmt* s2 = m->elseStatements[i];
		write(s2,file); 
	}
}

void write(struct WhileStmt* m, FILE* file){

	write(m->condition,file);

	for(int i = 0;i < (m->count_statements);i++){ 
		struct Stmt* s = m->statements[i];
		write(s,file); 
	}
}

void write(struct RetStmt* m, FILE* file){
	write(m->returnValue,file);
}

void write(struct AssignStmt* m, FILE* file){
	if(m->optTypeNode != NULL){
		write(m->optTypeNode,file);
	}else{
		fprintf(file,"NULL\t");
	}
	write(m->variableNode,file);
	write(m->expressionNode,file);
}
void write(struct MethodCall* m, FILE* file){
	fprintf(file,"%s\t",m->methodName);
	for(int i=0;i < m->count_args;i++){ 
		struct Expr* e = m->args[i];
		write(e,file); 
	}
}
// --------- TYPENODES --------------
void write(struct Type* m, FILE* file){
	//there is an alternative. we give a small number to indicate the alternative
	
	if(m->m1 != NULL){
		fprintf(file,"1\t");
		write(m->m1,file);

	}else if(m->m2 != NULL){
		fprintf(file,"2\t");
		write(m->m2,file);

	}else if(m->m3 != NULL){
		fprintf(file,"3\t");
		write(m->m3,file);
	}
}

void write(struct ArrayType* m, FILE* file){
	write(m->element_type,file);
}

void write(struct TypeParam* m, FILE* file){
	fprintf(file,"%d\t",m->typeParameterIndex);
}

void write(struct BasicTypeWrapped* m, FILE* file){
	if(m->m1 != NULL){ fprintf(file,"1\t"); write(m->m1,file); }

	if(m->m2 != NULL){ fprintf(file,"2\t"); write(m->m2,file); }
}

void write(struct SimpleType* m, FILE* file){
	fprintf(file,"%s\t",m->typeName);
}

void write(struct SubrType* m, FILE* file){
	write(m->returnType,file);
	fprintf(file,"%s\t",m->hasSideEffects);

	for(int i = 0;i < m->count_argumentTypes;i++){
		struct Type* t = m->argumentTypes[i];
		write(t,file); 
	}
}
// --------- OTHER ----------
void write_ast(char* filename, struct Namespace* namespaceNode){
	FILE* file;
	file = fopen(filename, "w");
	
	write(namespaceNode,&file);
	
	fclose(file);
}