#ifndef IFSTATEMENTNODE
#define IFSTATEMENTNODE

//standard headers
#include <vector>

//project headers
class ExpressionNode;
class StatementNode;
#include "../../commandline/TokenList.hpp"

class IfStatementNode{

public:
	IfStatementNode(TokenList tokens);

	ExpressionNode* condition;
	vector<StatementNode*> statements;
	vector<StatementNode*> elseStatements;
};

#endif