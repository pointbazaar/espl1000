#ifndef INTCONSTNODE
#define INTCONSTNODE

#include "ITermNode.hpp"
#include "../commandline/TokenList.hpp"

using namespace std;

class IntConstNode : public ITermNode {

public:

	IntConstNode(TokenList tokens);

	int number;
};

#endif