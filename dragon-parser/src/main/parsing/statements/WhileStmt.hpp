#ifndef WHILESTMT
#define WHILESTMT

class TokenList;

struct Expr;
struct Stmt;

struct WhileStmt  {

	struct Expr* condition;

	int count_statements = 0;
	struct Stmt** statements;
};
	
struct WhileStmt* makeWhileStmt(TokenList* tokens,bool debug);

#endif