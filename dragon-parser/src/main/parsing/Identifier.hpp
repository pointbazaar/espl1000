#ifndef IDENTIFIER
#define IDENTIFIER

#include <string>

#include "../commandline/TokenList.hpp"

class Identifier  {

public:
	Identifier(TokenList* tokens, bool debug);

	std::string identifier;
};
#endif