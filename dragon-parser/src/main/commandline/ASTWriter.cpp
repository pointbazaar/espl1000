#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ASTWriter.hpp"

void write(Namespace nsn, ofstream* file){
	*file << nsn.srcPath;
	*file << "\t";
	*file << nsn.name;
	*file << "\t";
	*file << nsn.methods.size();
	for(Method m : nsn.methods){ write(m,file); }
}
void write(Method m, ofstream* file){
	*file << m.isPublic;
	*file << "\t";
	*file << m.hasSideEffects;
	*file << "\t";
	write( *(m.returnType),file);
	*file << m.methodName;
	*file << "\t";
	*file << m.arguments.size();
	for(DeclArg* arg : m.arguments){ write(*arg,file); }
	*file << m.statements.size();
	for(Stmt* s : m.statements){ write(*s,file); }
}
void write(DeclArg m, ofstream* file){
	write(*(m.type),file);
	if(m.name.has_value()){
		*file << m.name.value();
	}else{
		*file << "NULL";
	}
	*file << "\t";
}
void write(struct Variable* m, ofstream* file){
	write(*(m->simpleVariableNode),file);
	for(struct Variable* v : m->memberAccessList){ write(v,file); }
}
void write(SimpleVar m, ofstream* file){
	*file << m.name << "\t";
	if(m.indexOptional != NULL){
		write(m.indexOptional,file);
	}else{
		*file << "NULL" << "\t";
	}
}
void write(struct Expr* m, ofstream* file){
	write(*(m->term1),file);
	if(m->op != NULL){
		write(*(m->op),file);
		write(*(m->term2),file);
	}else{
		*file << "NULL" << "\t";
	}
}
void write(Term m, ofstream* file){
	if(m.m1 != NULL){ *file << "1" << "\t"; write(*(m.m1),file); }
	if(m.m2 != NULL){ *file << "2" << "\t"; write(*(m.m2),file); }
	if(m.m3 != NULL){ *file << "3" << "\t"; write(*(m.m3),file); }
	if(m.m4 != NULL){ *file << "4" << "\t"; write(*(m.m4),file); }
	if(m.m5 != NULL){ *file << "5" << "\t"; write(m.m5,file); }
	if(m.m6 != NULL){ *file << "6" << "\t"; write(m.m6,file); }
}
void write(BoolConst m, ofstream* file){
	*file << m.boolValue << "\t";
}
void write(IntConst m, ofstream* file){
	*file << m.number << "\t";
}
void write(CharConst m, ofstream* file){
	*file << m.content << "\t";
}
void write(Op m, ofstream* file){
	*file << m.op << "\t";
}
// ---------------- STATEMENTNODES ---------------------
void write(Stmt m, ofstream* file){
	//the reader has to know which type it is,
	//we can print a small number
	if(m.m1 != NULL){*file << "1" << "\t"; write(*m.m1,file); }
	if(m.m2 != NULL){*file << "2" << "\t"; write(*m.m2,file); }
	if(m.m3 != NULL){*file << "3" << "\t"; write(*m.m3,file); }
	if(m.m4 != NULL){*file << "4" << "\t"; write(*m.m4,file); }
	if(m.m5 != NULL){*file << "5" << "\t"; write(*m.m5,file); }
}
void write(IfStmt m, ofstream* file){
	write(m.condition,file);
	for(Stmt* s : m.statements){ write(*s,file); }
	for(Stmt* s2 : m.elseStatements){ write(*s2,file); }
}
void write(WhileStmt m, ofstream* file){
	write(m.condition,file);
	for(Stmt* s : m.statements){ write(*s,file); }
}
void write(RetStmt m, ofstream* file){
	write(m.returnValue,file);
}
void write(AssignStmt m, ofstream* file){
	if(m.optTypeNode.has_value()){
		write(*(m.optTypeNode.value()),file);
	}else{
		*file << "NULL" << "\t";
	}
	write(m.variableNode,file);
	write(m.expressionNode,file);
}
void write(MethodCall m, ofstream* file){
	*file << m.methodName << "\t";
	for(struct Expr* e : m.arguments){ write(e,file); }
}
// --------- TYPENODES --------------
void write(Type m, ofstream* file){
	//there is an alternative. we give a small number to indicate the alternative
	if(m.m1 != NULL){
		*file << "1" << "\t";
		write(*(m.m1),file);
	}else if(m.m2 != NULL){
		*file << "2" << "\t";
		write(*(m.m2),file);
	}else if(m.m3 != NULL){
		*file << "3" << "\t";
		write(*(m.m3),file);
	}
}
void write(ArrayType m, ofstream* file){
	write(*(m.element_type),file);
}
void write(TypeParam m, ofstream* file){
	*file << m.typeParameterIndex << "\t";
}
void write(BasicTypeWrapped m, ofstream* file){
	if(m.m1 != NULL){ *file << "1\t"; write(*(m.m1),file); }
	if(m.m2 != NULL){ *file << "2\t"; write(*(m.m2),file); }
}
void write(SimpleType m, ofstream* file){
	*file << m.typeName << "\t";
}
void write(SubrType m, ofstream* file){
	write(*(m.returnType),file);
	*file << m.hasSideEffects << "\t";
	for(Type* t : m.argumentTypes){ write(*t,file); }
}
// --------- OTHER ----------
void write_ast(string filename, Namespace namespaceNode){
	ofstream file;
	file.open(filename, ios::out);
	
	write(namespaceNode,&file);
	
	file.close();
}