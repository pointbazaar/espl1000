#ifndef DECLAREDARG
#define DECLAREDARG

#include <optional>
#include <string>

class Type;
#include "../commandline/TokenList.hpp"

class DeclaredArg  {

public:

	DeclaredArg(TokenList* tokens, bool debug);

	Type* type = NULL;
	optional<std::string> name;
};

#endif