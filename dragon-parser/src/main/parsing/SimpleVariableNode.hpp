#ifndef SIMPLEVARIABLENODE
#define SIMPLEVARIABLENODE

#include <string>
#include <optional>

class ExpressionNode;
#include "../commandline/TokenList.hpp"

class SimpleVariableNode {

public:
	SimpleVariableNode(TokenList tokens, bool debug);

	std::string name;
	std::optional<ExpressionNode*> indexOptional;
};

#endif