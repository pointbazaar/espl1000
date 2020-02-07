#ifndef IFSTATEMENTNODE
#define IFSTATEMENTNODE

//standard headers
#include <vector>
#include <iostream>
//project headers
class ExpressionNode;
class StatementNode;
#include "../../commandline/TokenList.hpp"

class IfStatementNode{

public:
	IfStatementNode(TokenList tokens,bool debug);

	ExpressionNode* condition;
	vector<StatementNode*> statements;
	vector<StatementNode*> elseStatements;
};

#endif