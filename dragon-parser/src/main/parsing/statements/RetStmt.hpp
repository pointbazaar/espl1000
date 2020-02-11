#ifndef RETURNSTMT
#define RETURNSTMT

class Expr;

#include "../../commandline/TokenList.hpp"

class RetStmt{

public:
	struct Expr* returnValue;
	RetStmt(TokenList list,bool debug);
};

#endif