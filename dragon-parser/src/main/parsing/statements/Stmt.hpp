#ifndef STMT
#define STMT

#include "../../commandline/TokenList.hpp"
class MethodCall;
class WhileStmt;
class IfStmt;
class RetStmt;
class AssignStmt;

class Stmt {

public:
	Stmt(TokenList tokens, bool debug);

	//only one of those will be != NULL
	MethodCall* m1 = NULL;
	WhileStmt* m2 = NULL;
	IfStmt* m3 = NULL;
	RetStmt* m4 = NULL;
	AssignStmt* m5 = NULL;
};
#endif