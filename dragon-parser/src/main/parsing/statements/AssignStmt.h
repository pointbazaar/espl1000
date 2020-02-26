#ifndef ASSIGNSTMT
#define ASSIGNSTMT

#include <stdbool.h>

struct TokenList;

struct Variable;
struct Expr;
struct Type;

struct AssignStmt {


	struct Type* optTypeNode;	//may be NULL (optional)

	struct Variable* variableNode;
	struct Expr* expressionNode;
};

struct AssignStmt* makeAssignStmt(struct TokenList* tokens,bool debug);

#endif