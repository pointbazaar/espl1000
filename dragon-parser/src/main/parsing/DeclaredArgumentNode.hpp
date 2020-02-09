#ifndef DECLAREDARGUMENTNODE
#define DECLAREDARGUMENTNODE

#include <optional>
#include <string>

class TypeNode;
#include "../commandline/TokenList.hpp"

class DeclaredArgumentNode  {

public:

	DeclaredArgumentNode(TokenList* tokens, bool debug);

	TypeNode* type = NULL;
	optional<std::string> name;
};

#endif