#ifndef IFSTMT
#define IFSTMT

class TokenList;

struct Expr;
struct Stmt;

struct IfStmt{
	struct Expr* condition = NULL;

	int count_statements = 0;
	Stmt** statements = NULL;

	int count_elseStatements = 0;
	Stmt** elseStatements = NULL;
};
	
struct IfStmt* makeIfStmt(TokenList* tokens,bool debug);

#endif