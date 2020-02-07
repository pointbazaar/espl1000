#ifndef METHODCALLNODE
#define METHODCALLNODE

#include <vector>
#include <string>

class ExpressionNode;
#include "../../commandline/TokenList.hpp"

class MethodCallNode {

public:
	MethodCallNode(TokenList tokens,bool debug);
	
	string methodName;
	vector<ExpressionNode*> arguments;
};

#endif