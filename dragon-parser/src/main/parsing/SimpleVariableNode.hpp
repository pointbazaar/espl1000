#ifndef SIMPLEVARIABLENODE
#define SIMPLEVARIABLENODE

#include <string>
#include <optional>
#include "ExpressionNode.hpp"

using namespace std;

class SimpleVariableNode {

public:
	string name;
	optional<ExpressionNode> indexOptional;

};

#endif