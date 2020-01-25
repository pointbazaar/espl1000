#ifndef METHODNODE
#define METHODNODE

#include <string>
#include <vector>

#include "DeclaredArgument.hpp"
#include "StatementNode.hpp"
#include "TypeNode.hpp"

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