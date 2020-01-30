#ifndef DECLAREDARGUMENTNODE
#define DECLAREDARGUMENTNODE

#include <optional>
#include <string>

#include "typenodes/TypeNode.hpp"

using namespace std;

class DeclaredArgumentNode  {

public:
	TypeNode type;
	optional<string> name;
};

#endif