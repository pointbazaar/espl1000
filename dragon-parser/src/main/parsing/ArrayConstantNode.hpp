#ifndef ARRAYCONSTANTNODE
#define ARRAYCONSTANTNODE

#include <vector>

#include "../lexing/TokenList.hpp"
#include "ExpressionNode.hpp"

using namespace std;

class ArrayConstantNode : ITermNode {

public:
	vector<ExpressionNode> elements;

	ArrayConstantNode(TokenList tokens);
};

#endif