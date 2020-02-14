#ifndef RETURNSTMT
#define RETURNSTMT

struct Expr;

#include "../../commandline/TokenList.hpp"

struct RetStmt{

	struct Expr* returnValue;
};

struct RetStmt* makeRetStmt(TokenList* list,bool debug);

#endif