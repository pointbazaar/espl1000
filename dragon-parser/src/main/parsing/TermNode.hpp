#ifndef TERMNODE
#define TERMNODE

#include <variant>

#include "ExpressionNode.hpp"
#include "BoolConstNode.hpp"
#include "CharConstNode.hpp"
#include "IntConstNode.hpp"
#include "VariableNode.hpp"
#include "ExpressionNode.hpp"
#include "statements/MethodCallNode.hpp"
#include "../commandline/TokenList.hpp"

using namespace std;

class TermNode {

public:

	TermNode ( ExpressionNode myexpr);
	
	TermNode ( TokenList tokens);

	variant<BoolConstNode,IntConstNode,CharConstNode,MethodCallNode,ExpressionNode,VariableNode> termNode;

};

#endif