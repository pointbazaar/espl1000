#ifndef STMT
#define STMT

#include <stdbool.h>

struct TokenList;
struct MethodCall;
struct WhileStmt;
struct IfStmt;
struct RetStmt;
struct AssignStmt;

struct Stmt {
	//only one of those will be != NULL
	struct MethodCall* m1;
	struct WhileStmt* m2;
	struct IfStmt* m3;
	struct RetStmt* m4;
	struct AssignStmt* m5;
};

struct Stmt* makeStmt(struct TokenList* tokens, bool debug);

#endif