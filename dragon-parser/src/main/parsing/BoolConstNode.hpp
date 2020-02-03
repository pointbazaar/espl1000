#ifndef BOOLCONSTNODE
#define BOOLCONSTNODE

#include "ITermNode.hpp"
#include "../lexing/TokenList.hpp"

using namespace std;

class BoolConstNode : public ITermNode {

public:
	BoolConstNode(TokenList tokens);

	bool boolValue;
};

#endif