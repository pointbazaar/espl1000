#ifndef EXPRESSIONNODE
#define EXPRESSIONNODE

#include <optional>

#include "TermNode.hpp"
#include "OperatorNode.hpp"
#include "ITermNode.hpp"

using namespace std;

class ExpressionNode : ITermNode {

public:
	//DragonExpressionNode should be similar to jack expression
	//an expression should be anything that returns a value or computes to a value

	TermNode term1;
	optional<OperatorNode> op;
	optional<TermNode> term2;
};

#endif