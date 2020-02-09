#ifndef VARIABLENODE
#define VARIABLENODE

#include <vector>

class SimpleVariableNode;
#include "../commandline/TokenList.hpp"

class VariableNode {

public:
	VariableNode(TokenList tokens, bool debug);

	SimpleVariableNode* simpleVariableNode;
	std::vector<VariableNode*> memberAccessList;
};

#endif