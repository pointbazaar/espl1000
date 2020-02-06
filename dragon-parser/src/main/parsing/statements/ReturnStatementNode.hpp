#ifndef RETURNSTATEMENTNODE
#define RETURNSTATEMENTNODE

class ExpressionNode;
#include "../../commandline/TokenList.hpp"

class ReturnStatementNode{

public:
	ExpressionNode* returnValue;
	ReturnStatementNode(TokenList list);
};

#endif