#ifndef METHODNODE
#define METHODNODE

#include <string>
#include <vector>

class DeclaredArgumentNode;
class StatementNode;
class TypeNode;
#include "../commandline/TokenList.hpp"

class MethodNode {

public:

	MethodNode(TokenList tokens, bool debug);

	bool isPublic = true;
	bool hasSideEffects;
	TypeNode* returnType;
	std::string methodName;
	std::vector<DeclaredArgumentNode*> arguments;
	std::vector<StatementNode*> statements;
};

#endif