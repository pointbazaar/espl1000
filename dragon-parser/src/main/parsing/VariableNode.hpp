#ifndef VARIABLENODE
#define VARIABLENODE

#include <vector>
#include "SimpleVariableNode.hpp"
#include "ITermNode.hpp"

using namespace std;

class VariableNode : ITermNode {

public:
	SimpleVariableNode simpleVariableNode;
	vector<VariableNode> memberAccessList;
};

#endif