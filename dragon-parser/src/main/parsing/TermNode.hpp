#ifndef TERMNODE
#define TERMNODE

#include <variant>

class ExpressionNode;
class BoolConstNode;
class CharConstNode;
class IntConstNode;
class VariableNode;
class ExpressionNode;
class MethodCallNode;

#include "../commandline/TokenList.hpp"

using namespace std;

class TermNode {

public:

	TermNode(ExpressionNode* myexpr);
	
	TermNode(TokenList tokens, bool debug);

	//only one of these may be != NULL
	BoolConstNode* m1 = NULL;
	IntConstNode* m2 = NULL;
	CharConstNode* m3 = NULL;
	MethodCallNode* m4 = NULL;
	ExpressionNode* m5 = NULL;
	VariableNode* m6 = NULL;

};

#endif