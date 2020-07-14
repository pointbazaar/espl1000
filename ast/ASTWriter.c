#include <stdio.h>
#include <stdlib.h>

#include "ASTWriter.h"

void writeNamespace(struct Namespace* nsn, FILE* file){
	printf("writeNamespace\n");
	
	fprintf(file,"%s\t%s\t%d\t",nsn->srcPath,nsn->name,nsn->count_methods);
	
	//write methods
	for(int i=0;i < nsn->count_methods;i++){ 
		struct Method* m = nsn->methods[i];
		writeMethod(m,file);
	}
	
	//write structs
	for(int i=0;i < nsn->count_structs;i++){ 
		struct StructDecl* m = nsn->structs[i];
		writeStructDecl(m,file);
	}
	
}
void writeMethod(struct Method* m, FILE* file){
	printf("writeMethod\n");
	
	fprintf(file, "Method\t");

	fprintf(file,"%d\t%d\t%s\t",m->isPublic,m->hasSideEffects,m->name);
	writeType(m->returnType,file);

	fprintf(file,"%d\t",m->count_args);
	for(int i = 0;i < m->count_args;i++){ 
		struct DeclArg* arg = m->args[i];
		writeDeclArg(arg,file); 
	}

	fprintf(file,"%d\t",m->count_stmts);
	for(int i = 0;i < m->count_stmts;i++){ 
		struct Stmt* s = m->stmts[i];
		writeStmt(s,file); 
	}
}
void writeStructDecl(struct StructDecl* m, FILE* file){
	printf("writeStructDecl\n");
	
	fprintf(file, "StructDecl\t");
	fprintf(file, "%d\t", m->count_members);
	for(int i=0;i < m->count_members;i++){
		writeStructMember(m->members[i], file);
	}
}
void writeStructMember(struct StructMember* m, FILE* file){
	fprintf(file, "StructMember\t");
	writeType(m->type, file);
	fprintf(file, "%s\t", m->name);
}
void writeDeclArg(struct DeclArg* m, FILE* file){
	fprintf(file, "DeclaredArg\t");

	writeType(m->type,file);
	if(m->name != NULL){
		fprintf(file,"%s\t",m->name);
	}else{
		fprintf(file,"NULL\t");
	}
}
void writeVariable(struct Variable* m, FILE* file){

	fprintf(file, "Variable\t");

	writeSimpleVar(m->simpleVar,file);

	fprintf(file, "%d\n", m->count_memberAccessList);
	for(int i = 0;i < m->count_memberAccessList;i++){ 
		struct Variable* v = m->memberAccessList[i];
		writeVariable(v, file); 
	}
}
void writeSimpleVar(struct SimpleVar* m, FILE* file){

	fprintf(file, "SimpleVariable\t");

	fprintf(file,"%s\t",m->name);
	if(m->optIndex != NULL){
		writeExpr(m->optIndex, file);
	}else{
		fprintf(file,"NULL\t");
	}
}
void writeExpr(struct Expr* m, FILE* file){

	fprintf(file, "Expr\t");

	writeTerm(m->term1, file);
	if(m->op != NULL){
		writeOp(m->op,file);
		writeTerm(m->term2,file);
	}else{
		fprintf(file,"NULL\t");
	}
}
void writeTerm(struct Term* m, FILE* file){

	fprintf(file, "Term\t");

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
	fprintf(file, "BoolConst\t");
	fprintf(file,"%d\t",m->value);
}
void writeIntConst(struct IntConst* m, FILE* file){
	fprintf(file, "IntConst\t");
	fprintf(file,"%d\t",m->value);
}
void writeCharConst(struct CharConst* m, FILE* file){
	fprintf(file, "CharConst\t");
	fprintf(file,"%c\t",m->value);
}
void writeFloatConst(struct FloatConst* m, 	FILE* file){
	fprintf(file, "FloatConst\t");
	fprintf(file,"%f\t",m->value);
}
void writeOp(struct Op* m, FILE* file){
	fprintf(file, "Op\t");

	fprintf(file,"%s\t",m->op);
}
// ---------------- STATEMENTNODES ---------------------
void writeStmt(struct Stmt* m, FILE* file){

	//to make it more human-readable / debuggable
	fprintf(file, "Stmt\t");

	//the reader has to know which type it is,
	//we can print a small number
	if(m->m1 != NULL){ fprintf(file,"1\t"); writeMethodCall(m->m1,file); }
	if(m->m2 != NULL){ fprintf(file,"2\t"); writeWhileStmt(m->m2,file); }
	if(m->m3 != NULL){ fprintf(file,"3\t"); writeIfStmt(m->m3,file); }
	if(m->m4 != NULL){ fprintf(file,"4\t"); writeRetStmt(m->m4,file); }
	if(m->m5 != NULL){ fprintf(file,"5\t"); writeAssignStmt(m->m5,file); }
}

void writeIfStmt(struct IfStmt* m, FILE* file){

	fprintf(file, "IfStmt\t");

	writeExpr(m->condition,file);

	fprintf(file, "%d\t", m->count_statements);
	for(int i = 0;i < m->count_statements;i++){ 
		struct Stmt* s = m->statements[i];
		writeStmt(s,file); 
	}

	fprintf(file, "%d\t", m->count_elseStatements);
	for(int i = 0;i < m->count_elseStatements;i++){ 
		struct Stmt* s2 = m->elseStatements[i];
		writeStmt(s2,file); 
	}
}

void writeWhileStmt(struct WhileStmt* m, FILE* file){

	fprintf(file, "WhileStmt\t");

	writeExpr(m->condition,file);

	fprintf(file, "%d\t", m->count_statements);
	for(int i = 0;i < (m->count_statements);i++){ 
		struct Stmt* s = m->statements[i];
		writeStmt(s,file); 
	}
}

void writeRetStmt(struct RetStmt* m, FILE* file){

	fprintf(file, "RetStmt\t");

	writeExpr(m->returnValue,file);
}

void writeAssignStmt(struct AssignStmt* m, FILE* file){

	fprintf(file, "AssignStmt\t");

	if(m->optType != NULL){
		writeType(m->optType, file);
	}else{
		fprintf(file,"NULL\t");
	}
	writeVariable(m->var, file);
	writeExpr(m->expr, file);
}
void writeMethodCall(struct MethodCall* m, FILE* file){

	fprintf(file, "MethodCall\t");

	fprintf(file, "%s\t", m->methodName);
	fprintf(file, "%d\t", m->count_args);
	for(int i=0;i < m->count_args;i++){ 
		struct Expr* e = m->args[i];
		writeExpr(e,file); 
	}
}
// --------- TYPENODES --------------
void writeType(struct Type* m, FILE* file){
	//there is an alternative. we give a small number to indicate the alternative
	fprintf(file, "Type\t");
	
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
	fprintf(file, "ArrayType\t");
	writeType(m->element_type,file);
}

void writeTypeParam(struct TypeParam* m, FILE* file){
	fprintf(file, "TypeParam\t");
	fprintf(file,"%d\t",m->index);
}

void writeBasicTypeWrapped(struct BasicTypeWrapped* m, FILE* file){
	fprintf(file, "BasicTypeWrapped\t");

	if(m->simpleType != NULL){ 
		fprintf(file,"1\t"); 
		writeSimpleType(m->simpleType,file); 
	}

	if(m->subrType != NULL){ 
		fprintf(file,"2\t"); 
		writeSubrType(m->subrType,file); 
	}
}

void writeSimpleType(struct SimpleType* m, FILE* file){
	fprintf(file, "SimpleType\t");
	fprintf(file,"%s\t",m->typeName);
}

void writeSubrType(struct SubrType* m, FILE* file){
	fprintf(file, "SubrType\t");

	writeType(m->returnType,file);
	fprintf(file,"%d\t",m->hasSideEffects);

	fprintf(file,"%d\t",m->count_argTypes);
	for(int i = 0;i < m->count_argTypes;i++){
		struct Type* t = m->argTypes[i];
		writeType(t,file); 
	}
}
// --------- OTHER ----------
void write_ast(char* filename, struct Namespace* namespaceNode){
	printf("write_ast\n");
	FILE* file;
	file = fopen(filename, "w");
	
	writeNamespace(namespaceNode, file);
	
	fclose(file);
}
