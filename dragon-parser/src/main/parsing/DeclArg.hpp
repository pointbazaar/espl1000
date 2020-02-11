#ifndef DECLARG
#define DECLARG

#include <optional>
#include <string>

class Type;
#include "../commandline/TokenList.hpp"

class DeclArg  {

public:

	DeclArg(TokenList* tokens, bool debug);

	Type* type = NULL;
	optional<std::string> name;
};

#endif