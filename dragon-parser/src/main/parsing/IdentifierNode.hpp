#ifndef IDENTIFIERNODE
#define IDENTIFIERNODE

#include <string>

#include "../lexing/TokenList.hpp"

using namespace std;

class IdentifierNode  {

public:

	IdentifierNode(TokenList tokens);

	string identifier;
};

#endif