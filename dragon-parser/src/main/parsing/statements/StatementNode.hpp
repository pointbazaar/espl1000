#ifndef STATEMENTNODE
#define STATEMENTNODE

#include "../../commandline/TokenList.hpp"
class MethodCallNode;
class WhileStatementNode;
class IfStatementNode;
class ReturnStatementNode;
class AssignmentStatementNode;

class StatementNode {

public:
	StatementNode(TokenList tokens);

	//only one of those will be != NULL
	MethodCallNode* m1 = NULL;
	WhileStatementNode* m2 = NULL;
	IfStatementNode* m3 = NULL;
	ReturnStatementNode* m4 = NULL;
	AssignmentStatementNode* m5 = NULL;
};

#endif