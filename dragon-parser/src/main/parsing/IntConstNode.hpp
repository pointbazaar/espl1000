#ifndef INTCONSTNODE
#define INTCONSTNODE

#include "../commandline/TokenList.hpp"

using namespace std;

class IntConstNode {

public:

	IntConstNode(TokenList tokens);

	int number;
};

#endif