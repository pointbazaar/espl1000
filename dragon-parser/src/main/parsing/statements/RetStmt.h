#ifndef RETURNSTMT
#define RETURNSTMT

#include <stdbool.h>

struct Expr;
struct TokenList;

struct RetStmt{

	struct Expr* returnValue;
};

struct RetStmt* makeRetStmt(struct TokenList* list,bool debug);

#endif