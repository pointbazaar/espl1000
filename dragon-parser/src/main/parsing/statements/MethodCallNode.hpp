#ifndef METHODCALLNODE
#define METHODCALLNODE

#include <vector>

#include "../ExpressionNode.hpp"
#include "IStatementNode.hpp"
#include "../ITermNode.hpp"
#include "../../commandline/TokenList.hpp"

class MethodCallNode : public IStatementNode, public ITermNode {

public:
	MethodCallNode(TokenList tokens);

	string methodName;
	vector<ExpressionNode> arguments;
};

#endif