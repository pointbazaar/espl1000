#ifndef WHILESTMT
#define WHILESTMT

#include <vector>

#include "../../commandline/TokenList.hpp"

class Expr;
class Stmt;

class WhileStmt  {

public:
	WhileStmt(TokenList tokens,bool debug);

	Expr* condition;
	vector<Stmt*> statements;
};

#endif