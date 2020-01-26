#ifndef IFSTATEMENTNODE
#define

//standard headers
#include <vector>

//project headers

using namespace std;

class IfStatementNode{
public:
	ExpressionNode condition;
	vector<StatementNode> statements;
	vector<StatementNode> elseStatements;
};

#endif