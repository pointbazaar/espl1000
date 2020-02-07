#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ASTWriter.hpp"

void write(NamespaceNode nsn, ofstream* file){
	*file << nsn.srcPath;
	*file << "\t";
	*file << nsn.name;
	*file << "\t";
	*file << nsn.methods.size();
	for(MethodNode m : nsn.methods){ write(m,file); }
}
void write(MethodNode m, ofstream* file){
	*file << m.isPublic;
	*file << "\t";
	*file << m.hasSideEffects;
	*file << "\t";
	write( *(m.returnType),file);
	*file << m.methodName;
	*file << "\t";
	*file << m.arguments.size();
	for(DeclaredArgumentNode* arg : m.arguments){ write(*arg,file); }
	*file << m.statements.size();
	for(StatementNode* s : m.statements){ write(*s,file); }
}
void write(DeclaredArgumentNode m, ofstream* file){
	write(*(m.type),file);
	if(m.name.has_value()){
		*file << m.name.value();
	}else{
		*file << "NULL";
	}
	*file << "\t";
}
void write(VariableNode m, ofstream* file){
	write(*(m.simpleVariableNode),file);
	for(VariableNode* v : m.memberAccessList){ write(*v,file); }
}
void write(SimpleVariableNode m, ofstream* file){
	*file << m.name << "\t";
	if(m.indexOptional.has_value()){
		write(*(m.indexOptional.value()),file);
	}else{
		*file << "NULL" << "\t";
	}
}
void write(ExpressionNode m, ofstream* file){
	write(*(m.term1),file);
	if(m.op.has_value()){
		write(*(m.op.value()),file);
		write(*(m.term2.value()),file);
	}else{
		*file << "NULL" << "\t";
	}
}
void write(TermNode m, ofstream* file){
	if(m.m1 != NULL){ *file << "1" << "\t"; write(*(m.m1),file); }
	if(m.m2 != NULL){ *file << "2" << "\t"; write(*(m.m2),file); }
	if(m.m3 != NULL){ *file << "3" << "\t"; write(*(m.m3),file); }
	if(m.m4 != NULL){ *file << "4" << "\t"; write(*(m.m4),file); }
	if(m.m5 != NULL){ *file << "5" << "\t"; write(*(m.m5),file); }
	if(m.m6 != NULL){ *file << "6" << "\t"; write(*(m.m6),file); }
}
void write(BoolConstNode m, ofstream* file){
	*file << m.boolValue << "\t";
}
void write(IntConstNode m, ofstream* file){
	*file << m.number << "\t";
}
void write(CharConstNode m, ofstream* file){
	*file << m.content << "\t";
}
void write(OperatorNode m, ofstream* file){
	*file << m.op << "\t";
}
// ---------------- STATEMENTNODES ---------------------
void write(StatementNode m, ofstream* file){
	//the reader has to know which type it is,
	//we can print a small number
	if(m.m1 != NULL){*file << "1" << "\t"; write(*m.m1,file); }
	if(m.m2 != NULL){*file << "2" << "\t"; write(*m.m2,file); }
	if(m.m3 != NULL){*file << "3" << "\t"; write(*m.m3,file); }
	if(m.m4 != NULL){*file << "4" << "\t"; write(*m.m4,file); }
	if(m.m5 != NULL){*file << "5" << "\t"; write(*m.m5,file); }
}
void write(IfStatementNode m, ofstream* file){
	write(*(m.condition),file);
	for(StatementNode* s : m.statements){ write(*s,file); }
	for(StatementNode* s2 : m.elseStatements){ write(*s2,file); }
}
void write(WhileStatementNode m, ofstream* file){
	write(*(m.condition),file);
	for(StatementNode* s : m.statements){ write(*s,file); }
}
void write(ReturnStatementNode m, ofstream* file){
	write(*(m.returnValue),file);
}
void write(AssignmentStatementNode m, ofstream* file){
	if(m.optTypeNode.has_value()){
		write(*(m.optTypeNode.value()),file);
	}else{
		*file << "NULL" << "\t";
	}
	write(*(m.variableNode),file);
	write(*(m.expressionNode),file);
}
void write(MethodCallNode m, ofstream* file){
	*file << m.methodName << "\t";
	for(ExpressionNode* e : m.arguments){ write(*e,file); }
}
// --------- TYPENODES --------------
void write(TypeNode m, ofstream* file){
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
void write(ArrayTypeNode m, ofstream* file){
	write(*(m.element_type),file);
}
void write(TypeParameterNode m, ofstream* file){
	*file << m.typeParameterIndex << "\t";
}
void write(BasicTypeWrappedNode m, ofstream* file){
	if(m.m1 != NULL){ *file << "1\t"; write(*(m.m1),file); }
	if(m.m2 != NULL){ *file << "2\t"; write(*(m.m2),file); }
}
void write(SimpleTypeNode m, ofstream* file){
	*file << m.typeName << "\t";
}
void write(SubroutineTypeNode m, ofstream* file){
	write(*(m.returnType),file);
	*file << m.hasSideEffects << "\t";
	for(TypeNode* t : m.argumentTypes){ write(*t,file); }
}
// --------- OTHER ----------
void write_ast(string filename, NamespaceNode namespaceNode){
	ofstream file;
	file.open(filename, ios::out);
	
	write(namespaceNode,&file);
	
	file.close();
}