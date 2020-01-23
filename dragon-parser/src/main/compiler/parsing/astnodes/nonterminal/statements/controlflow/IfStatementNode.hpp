#ifndef IFSTATEMENTNODE
#define

//standard headers
#include <vector>

//project headers

class IfStatementNode{
public:
	ExpressionNode condition;

	vector<StatementNode> statements;

	vector<StatementNode> elseStatements;
}

#endif