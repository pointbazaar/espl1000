#ifndef VARIABLENODE
#define VARIABLENODE

#include <vector>

#include "SimpleVariableNode.hpp"
#include "ITermNode.hpp"
#include "../lexing/TokenList.hpp"

using namespace std;

class VariableNode : ITermNode {

public:
	VariableNode(TokenList tokens);

	SimpleVariableNode simpleVariableNode;
	vector<VariableNode> memberAccessList;
};

#endif