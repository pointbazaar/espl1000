#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ASTWriter.hpp"

void write(struct Namespace* nsn, ofstream* file){
	*file << nsn->srcPath;
	*file << "\t";
	*file << nsn->name;
	*file << "\t";
	*file << nsn->count_methods;
	for(int i=0;i < nsn->count_methods;i++){ 
		struct Method* m = nsn->methods[i];
		write(m,file); 
	}
}
void write(struct Method* m, ofstream* file){
	*file << m->isPublic;
	*file << "\t";
	*file << m->hasSideEffects;
	*file << "\t";
	write(m->returnType,file);
	*file << m->methodName;
	*file << "\t";

	*file << m->count_arguments;
	for(int i = 0;i < m->count_arguments;i++){ 
		struct DeclArg* arg = m->arguments[i];
		write(arg,file); 
	}

	*file << m->count_statements;
	for(int i = 0;i < m->count_statements;i++){ 
		struct Stmt* s = m->statements[i];
		write(s,file); 
	}
}
void write(struct DeclArg* m, ofstream* file){
	write(m->type,file);
	if(m->name != NULL){
		*file << m->name;
	}else{
		*file << "NULL";
	}
	*file << "\t";
}
void write(struct Variable* m, ofstream* file){
	write(m->simpleVariableNode,file);
	for(int i = 0;i < m->count_memberAccessList;i++){ 
		struct Variable* v = m->memberAccessList[i];
		write(v,file); 
	}
}
void write(struct SimpleVar* m, ofstream* file){
	*file << m->name << "\t";
	if(m->indexOptional != NULL){
		write(m->indexOptional,file);
	}else{
		*file << "NULL" << "\t";
	}
}
void write(struct Expr* m, ofstream* file){
	write(m->term1,file);
	if(m->op != NULL){
		write(m->op,file);
		write(m->term2,file);
	}else{
		*file << "NULL" << "\t";
	}
}
void write(struct Term* m, ofstream* file){
	if(m->m1 != NULL){ *file << "1" << "\t"; write(m->m1,file); }
	if(m->m2 != NULL){ *file << "2" << "\t"; write(m->m2,file); }
	if(m->m3 != NULL){ *file << "3" << "\t"; write(m->m3,file); }
	if(m->m4 != NULL){ *file << "4" << "\t"; write(m->m4,file); }
	if(m->m5 != NULL){ *file << "5" << "\t"; write(m->m5,file); }
	if(m->m6 != NULL){ *file << "6" << "\t"; write(m->m6,file); }
}
void write(struct BoolConst* m, ofstream* file){
	*file << m->boolValue << "\t";
}
void write(struct IntConst* m, ofstream* file){
	*file << m->number << "\t";
}
void write(struct CharConst* m, ofstream* file){
	*file << m->content << "\t";
}
void write(struct Op* m, ofstream* file){
	*file << m->op << "\t";
}
// ---------------- STATEMENTNODES ---------------------
void write(struct Stmt* m, ofstream* file){
	//the reader has to know which type it is,
	//we can print a small number
	if(m->m1 != NULL){*file << "1" << "\t"; write(m->m1,file); }
	if(m->m2 != NULL){*file << "2" << "\t"; write(m->m2,file); }
	if(m->m3 != NULL){*file << "3" << "\t"; write(m->m3,file); }
	if(m->m4 != NULL){*file << "4" << "\t"; write(m->m4,file); }
	if(m->m5 != NULL){*file << "5" << "\t"; write(m->m5,file); }
}

void write(struct IfStmt* m, ofstream* file){
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

void write(struct WhileStmt* m, ofstream* file){

	write(m->condition,file);

	for(int i = 0;i < (m->count_statements);i++){ 
		struct Stmt* s = m->statements[i];
		write(s,file); 
	}
}

void write(struct RetStmt* m, ofstream* file){
	write(m->returnValue,file);
}

void write(struct AssignStmt* m, ofstream* file){
	if(m->optTypeNode != NULL){
		write(m->optTypeNode,file);
	}else{
		*file << "NULL" << "\t";
	}
	write(m->variableNode,file);
	write(m->expressionNode,file);
}
void write(struct MethodCall* m, ofstream* file){
	*file << m->methodName << "\t";
	for(int i=0;i < m->count_args;i++){ 
		struct Expr* e = m->args[i];
		write(e,file); 
	}
}
// --------- TYPENODES --------------
void write(struct Type* m, ofstream* file){
	//there is an alternative. we give a small number to indicate the alternative
	if(m->m1 != NULL){
		*file << "1" << "\t";
		write(m->m1,file);

	}else if(m->m2 != NULL){
		*file << "2" << "\t";
		write(m->m2,file);

	}else if(m->m3 != NULL){
		*file << "3" << "\t";
		write(m->m3,file);
	}
}

void write(struct ArrayType* m, ofstream* file){
	write(m->element_type,file);
}

void write(struct TypeParam* m, ofstream* file){
	*file << m->typeParameterIndex << "\t";
}

void write(struct BasicTypeWrapped* m, ofstream* file){
	if(m->m1 != NULL){ *file << "1\t"; write(m->m1,file); }

	if(m->m2 != NULL){ *file << "2\t"; write(m->m2,file); }
}

void write(struct SimpleType* m, ofstream* file){
	*file << m->typeName << "\t";
}

void write(struct SubrType* m, ofstream* file){
	write(m->returnType,file);
	*file << m->hasSideEffects << "\t";

	for(int i = 0;i < m->count_argumentTypes;i++){
		struct Type* t = m->argumentTypes[i];
		write(t,file); 
	}
}
// --------- OTHER ----------
void write_ast(string filename, struct Namespace* namespaceNode){
	ofstream file;
	file.open(filename, ios::out);
	
	write(namespaceNode,&file);
	
	file.close();
}