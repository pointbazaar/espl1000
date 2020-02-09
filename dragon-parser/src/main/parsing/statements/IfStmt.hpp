#ifndef IFSTMT
#define IFSTMT

#include <vector>
#include <iostream>

#include "../../commandline/TokenList.hpp"

class Expr;
class Stmt;

class IfStmt{

public:
	IfStmt(TokenList tokens,bool debug);

	Expr* condition;
	vector<Stmt*> statements;
	vector<Stmt*> elseStatements;
};

#endif