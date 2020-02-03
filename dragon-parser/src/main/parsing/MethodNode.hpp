#include <string>
#include <optional>
#include <vector>

#include "DeclaredArgumentNode.hpp"
#include "statements/StatementNode.hpp"
#include "typenodes/TypeNode.hpp"
#include "../lexing/TokenList.hpp"

#ifndef METHODNODE
#define METHODNODE

using namespace std;

class MethodNode {

public:

	MethodNode(TokenList tokens, bool debug);

	bool isPublic = true;
	bool hasSideEffects;
	TypeNode returnType;
	string methodName;
	vector<DeclaredArgumentNode> arguments;
	vector<StatementNode> statements;
};

#endif