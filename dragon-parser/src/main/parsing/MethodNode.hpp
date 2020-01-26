#include <string>
#include <optional>
#include <vector>

#include "DeclaredArgumentNode.hpp"
#include "statements/StatementNode.hpp"
#include "typenodes/TypeNode.hpp"

#ifndef METHODNODE
#define METHODNODE

class MethodNode {

public:
	bool isPublic = true;
	bool hasSideEffects;
	TypeNode returnType;
	string methodName;
	vector<DeclaredArgumentNode> arguments;
	vector<StatementNode> statements;
};

#endif