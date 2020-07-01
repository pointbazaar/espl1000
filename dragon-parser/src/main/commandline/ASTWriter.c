#include <stdio.h>
#include <stdlib.h>

#include "ASTWriter.h"

void writeNamespace(struct Namespace* nsn, FILE* file){
	fprintf(file,"%s\t%s\t%d\t",nsn->srcPath,nsn->name,nsn->count_methods);
	
	for(int i=0;i < nsn->count_methods;i++){ 
		struct Method* m = nsn->methods[i];
		writeMethod(m,file);
	}
}
void writeMethod(struct Method* m, FILE* file){
	fprintf(file,"%d\t%d\t%s\t",m->isPublic,m->hasSideEffects,m->methodName);
	writeType(m->returnType,file);

	fprintf(file,"%d\t",m->count_arguments);
	for(int i = 0;i < m->count_arguments;i++){ 
		struct DeclArg* arg = m->arguments[i];
		writeDeclArg(arg,file); 
	}

	fprintf(file,"%d\t",m->count_statements);
	for(int i = 0;i < m->count_statements;i++){ 
		struct Stmt* s = m->statements[i];
		writeStmt(s,file); 
	}
}
void writeDeclArg(struct DeclArg* m, FILE* file){
	writeType(m->type,file);
	if(m->name != NULL){
		fprintf(file,"%s\t",m->name);
	}else{
		fprintf(file,"NULL\t");
	}
}
void writeVariable(struct Variable* m, FILE* file){
	writeSimpleVar(m->simpleVariableNode,file);
	for(int i = 0;i < m->count_memberAccessList;i++){ 
		struct Variable* v = m->memberAccessList[i];
		writeSimpleVar(v->simpleVariableNode,file); 
	}
}
void writeSimpleVar(struct SimpleVar* m, FILE* file){
	fprintf(file,"%s\t",m->name);
	if(m->indexOptional != NULL){
		writeExpr(m->indexOptional, file);
	}else{
		fprintf(file,"NULL\t");
	}
}
void writeExpr(struct Expr* m, FILE* file){
	writeTerm(m->term1, file);
	if(m->op != NULL){
		writeOp(m->op,file);
		writeTerm(m->term2,file);
	}else{
		fprintf(file,"NULL\t");
	}
}
void writeTerm(struct Term* m, FILE* file){
	if(m->m1 != NULL){ 
		fprintf(file,"1\t"); writeBoolConst(m->m1,file);
	}
	if(m->m2 != NULL){ 
		fprintf(file,"2\t"); writeIntConst(m->m2,file); 
	}
	if(m->m3 != NULL){ 
		fprintf(file,"3\t"); writeCharConst(m->m3,file); 
	}
	if(m->m4 != NULL){ 
		fprintf(file,"4\t"); writeMethodCall(m->m4,file); 
	}
	if(m->m5 != NULL){ 
		fprintf(file,"5\t"); writeExpr(m->m5,file); 
	}
	if(m->m6 != NULL){ 
		fprintf(file,"6\t"); writeVariable(m->m6,file); 
	}
	if(m->m7 != NULL){
		fprintf(file,"7\t"); writeFloatConst(m->m7,file); 
	}
}
void writeBoolConst(struct BoolConst* m, FILE* file){
	fprintf(file,"%d\t",m->value);
}
void writeIntConst(struct IntConst* m, FILE* file){
	fprintf(file,"%d\t",m->value);
}
void writeCharConst(struct CharConst* m, FILE* file){
	fprintf(file,"%c\t",m->content);
}
void writeFloatConst(struct FloatConst* m, 	FILE* file){
	fprintf(file,"%f\t",m->value);
}
void writeOp(struct Op* m, FILE* file){
	fprintf(file,"%s\t",m->op);
}
// ---------------- STATEMENTNODES ---------------------
void writeStmt(struct Stmt* m, FILE* file){
	//the reader has to know which type it is,
	//we can print a small number
	if(m->m1 != NULL){ fprintf(file,"1\t"); writeMethodCall(m->m1,file); }
	if(m->m2 != NULL){ fprintf(file,"2\t"); writeWhileStmt(m->m2,file); }
	if(m->m3 != NULL){ fprintf(file,"3\t"); writeIfStmt(m->m3,file); }
	if(m->m4 != NULL){ fprintf(file,"4\t"); writeRetStmt(m->m4,file); }
	if(m->m5 != NULL){ fprintf(file,"5\t"); writeAssignStmt(m->m5,file); }
}

void writeIfStmt(struct IfStmt* m, FILE* file){
	writeExpr(m->condition,file);
	for(int i = 0;i < m->count_statements;i++){ 
		struct Stmt* s = m->statements[i];
		writeStmt(s,file); 
	}

	for(int i = 0;i < m->count_elseStatements;i++){ 
		struct Stmt* s2 = m->elseStatements[i];
		writeStmt(s2,file); 
	}
}

void writeWhileStmt(struct WhileStmt* m, FILE* file){

	writeExpr(m->condition,file);

	for(int i = 0;i < (m->count_statements);i++){ 
		struct Stmt* s = m->statements[i];
		writeStmt(s,file); 
	}
}

void writeRetStmt(struct RetStmt* m, FILE* file){
	writeExpr(m->returnValue,file);
}

void writeAssignStmt(struct AssignStmt* m, FILE* file){
	if(m->optTypeNode != NULL){
		writeType(m->optTypeNode,file);
	}else{
		fprintf(file,"NULL\t");
	}
	writeVariable(m->variableNode,file);
	writeExpr(m->expressionNode,file);
}
void writeMethodCall(struct MethodCall* m, FILE* file){
	fprintf(file,"%s\t",m->methodName);
	for(int i=0;i < m->count_args;i++){ 
		struct Expr* e = m->args[i];
		writeExpr(e,file); 
	}
}
// --------- TYPENODES --------------
void writeType(struct Type* m, FILE* file){
	//there is an alternative. we give a small number to indicate the alternative
	
	if(m->m1 != NULL){
		fprintf(file,"1\t");
		writeBasicTypeWrapped(m->m1,file);

	}else if(m->m2 != NULL){
		fprintf(file,"2\t");
		writeTypeParam(m->m2,file);

	}else if(m->m3 != NULL){
		fprintf(file,"3\t");
		writeArrayType(m->m3,file);
	}
}

void writeArrayType(struct ArrayType* m, FILE* file){
	writeType(m->element_type,file);
}

void writeTypeParam(struct TypeParam* m, FILE* file){
	fprintf(file,"%d\t",m->typeParameterIndex);
}

void writeBasicTypeWrapped(struct BasicTypeWrapped* m, FILE* file){
	if(m->m1 != NULL){ 
		fprintf(file,"1\t"); writeSimpleType(m->m1,file); 
	}

	if(m->m2 != NULL){ 
		fprintf(file,"2\t"); writeSubrType(m->m2,file); 
	}
}

void writeSimpleType(struct SimpleType* m, FILE* file){
	fprintf(file,"%s\t",m->typeName);
}

void writeSubrType(struct SubrType* m, FILE* file){
	writeType(m->returnType,file);
	fprintf(file,"%d\t",m->hasSideEffects);

	for(int i = 0;i < m->count_argumentTypes;i++){
		struct Type* t = m->argumentTypes[i];
		writeType(t,file); 
	}
}
// --------- OTHER ----------
void write_ast(char* filename, struct Namespace* namespaceNode){
	FILE* file;
	file = fopen(filename, "w");
	
	writeNamespace(namespaceNode, file);
	
	fclose(file);
}
