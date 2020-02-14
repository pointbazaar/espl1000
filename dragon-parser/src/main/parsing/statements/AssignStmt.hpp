#ifndef ASSIGNSTMT
#define ASSIGNSTMT

class TokenList;

struct Variable;
struct Expr;
struct Type;

struct AssignStmt {


	struct Type* optTypeNode;	//may be NULL (optional)

	struct Variable* variableNode;
	struct Expr* expressionNode;
};

struct AssignStmt* makeAssignStmt(TokenList* tokens,bool debug);

#endif