#ifndef STMT
#define STMT

class TokenList;

struct MethodCall;
struct WhileStmt;
struct IfStmt;
struct RetStmt;
struct AssignStmt;

struct Stmt {
	//only one of those will be != NULL
	struct MethodCall* m1 = NULL;
	struct WhileStmt* m2 = NULL;
	struct IfStmt* m3 = NULL;
	struct RetStmt* m4 = NULL;
	struct AssignStmt* m5 = NULL;
};

struct Stmt* makeStmt(TokenList* tokens, bool debug);

#endif