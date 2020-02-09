#ifndef INTCONSTNODE
#define INTCONSTNODE

#include "../commandline/TokenList.hpp"

class IntConstNode {

public:

	IntConstNode(TokenList tokens, bool debug);

	int number;
};

#endif