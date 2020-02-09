#ifndef BOOLCONSTNODE
#define BOOLCONSTNODE

#include "../commandline/TokenList.hpp"

class BoolConstNode {

public:
	BoolConstNode(TokenList tokens, bool debug);

	bool boolValue;
};

#endif