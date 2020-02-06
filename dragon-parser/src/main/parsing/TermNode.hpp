#ifndef TERMNODE
#define TERMNODE

#include "ITermNode.hpp"
#include "ExpressionNode.hpp"
#include "../commandline/TokenList.hpp"

using namespace std;

class TermNode {

public:

	TermNode ( ExpressionNode myexpr);
	
	TermNode ( TokenList tokens);

	ITermNode termNode;

};

#endif