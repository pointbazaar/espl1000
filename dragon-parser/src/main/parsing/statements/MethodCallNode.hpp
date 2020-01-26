#ifndef METHODCALLNODE
#define METHODCALLNODE

#include "IASTNode.hpp"
#include "IStatementNode.hpp"
#include "ITermNode.hpp"


class MethodCallNode : IASTNode, IStatementNode, ITermNode {

public:
	string methodName;

	vector<ExpressionNode> arguments;
};

#endif