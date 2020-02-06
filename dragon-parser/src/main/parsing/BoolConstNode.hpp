#ifndef BOOLCONSTNODE
#define BOOLCONSTNODE

#include "../commandline/TokenList.hpp"

using namespace std;

class BoolConstNode {

public:
	BoolConstNode(TokenList tokens);

	bool boolValue;
};

#endif