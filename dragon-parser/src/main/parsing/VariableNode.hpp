#ifndef VARIABLENODE
#define VARIABLENODE

#include <vector>

#include "SimpleVariableNode.hpp"
#include "ITermNode.hpp"
#include "../commandline/TokenList.hpp"

using namespace std;

class VariableNode : public ITermNode {

public:
	VariableNode(TokenList tokens);

	SimpleVariableNode simpleVariableNode;
	vector<VariableNode> memberAccessList;
};

#endif