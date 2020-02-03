#ifndef INTCONSTNODE
#define INTCONSTNODE

#include "ITermNode.hpp"
#include "../lexing/TokenList.hpp"

using namespace std;

class IntConstNode : ITermNode {

public:

	IntConstNode(TokenList tokens);

	int number;
};

#endif