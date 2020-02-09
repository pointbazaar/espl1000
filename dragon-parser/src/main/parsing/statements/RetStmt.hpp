#ifndef RETURNSTMT
#define RETURNSTMT

class Expr;

#include "../../commandline/TokenList.hpp"

class RetStmt{

public:
	Expr* returnValue;
	RetStmt(TokenList list,bool debug);
};

#endif