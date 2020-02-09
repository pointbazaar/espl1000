#ifndef IDENTIFIERNODE
#define IDENTIFIERNODE

#include <string>

#include "../commandline/TokenList.hpp"

class IdentifierNode  {

public:

	IdentifierNode(TokenList* tokens, bool debug);

	std::string identifier;
};

#endif