
#include <optional>
#include <string>

#include "typenodes/TypeNode.hpp"

#ifndef DECLAREDARGUMENTNODE
#define DECLAREDARGUMENTNODE


class DeclaredArgumentNode  {

public:
	TypeNode type;
	optional<string> name;
};

#endif