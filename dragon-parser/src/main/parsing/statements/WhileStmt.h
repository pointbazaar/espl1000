#ifndef WHILESTMT
#define WHILESTMT

#include <stdbool.h>

struct TokenList;
struct Expr;
struct Stmt;

struct WhileStmt  {

	struct Expr* condition;

	int count_statements;
	struct Stmt** statements;
};
	
struct WhileStmt* makeWhileStmt(struct TokenList* tokens,bool debug);

#endif