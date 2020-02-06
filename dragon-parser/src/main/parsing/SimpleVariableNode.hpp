#ifndef SIMPLEVARIABLENODE
#define SIMPLEVARIABLENODE

#include <string>
#include <optional>

class ExpressionNode;
#include "../commandline/TokenList.hpp"

using namespace std;

class SimpleVariableNode {

public:
	SimpleVariableNode(TokenList tokens);

	string name;
	optional<ExpressionNode*> indexOptional;
};

#endif