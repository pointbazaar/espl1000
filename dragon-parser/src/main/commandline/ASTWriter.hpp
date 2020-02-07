#ifndef ASTWRITER
#define ASTWRITER

//../parsing
#include "../parsing/AST_Whole_Program.hpp"
#include "../parsing/NamespaceNode.hpp"
#include "../parsing/MethodNode.hpp"
#include "../parsing/DeclaredArgumentNode.hpp"
#include "../parsing/BoolConstNode.hpp"
#include "../parsing/CharConstNode.hpp"
#include "../parsing/ExpressionNode.hpp"
#include "../parsing/IdentifierNode.hpp"
#include "../parsing/IntConstNode.hpp"
#include "../parsing/OperatorNode.hpp"
#include "../parsing/SimpleVariableNode.hpp"
#include "../parsing/TermNode.hpp"
#include "../parsing/VariableNode.hpp"

//statements
#include "../parsing/statements/IfStatementNode.hpp"
#include "../parsing/statements/WhileStatementNode.hpp"
#include "../parsing/statements/ReturnStatementNode.hpp"
#include "../parsing/statements/AssignmentStatementNode.hpp"
#include "../parsing/statements/StatementNode.hpp"
#include "../parsing/statements/MethodCallNode.hpp"

//typenodes
#include "../parsing/typenodes/ArrayTypeNode.hpp"
#include "../parsing/typenodes/BasicTypeWrappedNode.hpp"
#include "../parsing/typenodes/SimpleTypeNode.hpp"
#include "../parsing/typenodes/SubroutineTypeNode.hpp"
#include "../parsing/typenodes/TypeNode.hpp"
#include "../parsing/typenodes/TypeParameterNode.hpp"

void write_ast(string filename, NamespaceNode namespaceNode);


void write(NamespaceNode m, ofstream* file);
void write(MethodNode m, ofstream* file);
void write(DeclaredArgumentNode m, ofstream* file);
void write(ExpressionNode m, ofstream* file);
void write(OperatorNode m, ofstream* file);
void write(IntConstNode m, ofstream* file);
void write(BoolConstNode m, ofstream* file);
void write(CharConstNode m, ofstream* file);

//statementnodes
void write(StatementNode m, ofstream* file);
void write(IfStatementNode m, ofstream* file);
void write(WhileStatementNode m, ofstream* file);
void write(ReturnStatementNode m, ofstream* file);
void write(AssignmentStatementNode m, ofstream* file);
void write(MethodCallNode m, ofstream* file);

//typenodes
void write(TypeNode m, ofstream* file);
void write(SubroutineTypeNode m, ofstream* file);
void write(SimpleTypeNode m, ofstream* file);

#endif