#ifndef IFSTMT
#define IFSTMT

#include <stdbool.h>

struct TokenList;
struct Expr;
struct Stmt;

struct IfStmt{
	struct Expr* condition;

	int count_statements;
	struct Stmt** statements;

	int count_elseStatements;
	struct Stmt** elseStatements;
};
	
struct IfStmt* makeIfStmt(struct TokenList* tokens,bool debug);

#endif