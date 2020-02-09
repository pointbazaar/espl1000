#ifndef OPERATORNODE
#define OPERATORNODE

#include <string>

#include "../commandline/TokenList.hpp"

class OperatorNode  {

public:
	OperatorNode(TokenList tokens, bool debug);

	std::string op;
};

#endif