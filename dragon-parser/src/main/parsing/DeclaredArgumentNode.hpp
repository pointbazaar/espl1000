#ifndef DECLAREDARGUMENTNODE
#define DECLAREDARGUMENTNODE

#include <optional>
#include <string>

class TypeNode;
#include "../commandline/TokenList.hpp"

using namespace std;

class DeclaredArgumentNode  {

public:

	DeclaredArgumentNode(TokenList tokens, bool debug);

	TypeNode* type = NULL;
	optional<string> name;
};

#endif