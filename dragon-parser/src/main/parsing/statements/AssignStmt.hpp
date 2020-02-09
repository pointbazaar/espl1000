#ifndef ASSIGNSTMT
#define ASSIGNSTMT

#include <optional>

#include "AssignStmt.hpp"
#include "../../commandline/TokenList.hpp"

class Variable;
class Expr;
class Type;

class AssignStmt {

public:
	AssignStmt(TokenList tokens,bool debug);
	
	optional<Type*> optTypeNode;
	Variable* variableNode;
	Expr* expressionNode;
};

#endif