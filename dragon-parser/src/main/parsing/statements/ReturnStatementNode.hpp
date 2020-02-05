#ifndef RETURNSTATEMENTNODE
#define RETURNSTATEMENTNODE

#include "../ExpressionNode.hpp"
#include "../../commandline/TokenList.hpp"

using namespace std;

class ReturnStatementNode{

public:
	ExpressionNode returnValue;
	ReturnStatementNode(TokenList list);
};

#endif