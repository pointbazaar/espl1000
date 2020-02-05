#ifndef SIMPLEVARIABLENODE
#define SIMPLEVARIABLENODE

#include <string>
#include <optional>

#include "ExpressionNode.hpp"
#include "../commandline/TokenList.hpp"

using namespace std;

class SimpleVariableNode {

public:
	SimpleVariableNode(TokenList tokens);

	string name;
	optional<ExpressionNode> indexOptional;
};

#endif