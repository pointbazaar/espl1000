#ifndef VARIABLENODE
#define VARIABLENODE

#include <vector>

class SimpleVariableNode;
#include "../commandline/TokenList.hpp"

using namespace std;

class VariableNode {

public:
	VariableNode(TokenList tokens);

	SimpleVariableNode* simpleVariableNode;
	vector<VariableNode*> memberAccessList;
};

#endif