#ifndef IDENTIFIERNODE
#define IDENTIFIERNODE

#include <string>

#include "../commandline/TokenList.hpp"

using namespace std;

class IdentifierNode  {

public:

	IdentifierNode(TokenList* tokens, bool debug);

	string identifier;
};

#endif