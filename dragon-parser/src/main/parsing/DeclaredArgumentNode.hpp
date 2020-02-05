#ifndef DECLAREDARGUMENTNODE
#define DECLAREDARGUMENTNODE

#include <optional>
#include <string>

#include "typenodes/TypeNode.hpp"
#include "../commandline/TokenList.hpp"

using namespace std;

class DeclaredArgumentNode  {

public:

	DeclaredArgumentNode(TokenList tokens);

	TypeNode type;
	optional<string> name;
};

#endif