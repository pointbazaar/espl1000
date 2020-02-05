#ifndef IFSTATEMENTNODE
#define IFSTATEMENTNODE

//standard headers
#include <vector>

//project headers
#include "../ExpressionNode.hpp"
#include "StatementNode.hpp"
#include "../../commandline/TokenList.hpp"

using namespace std;

class IfStatementNode{
public:
	IfStatementNode(TokenList tokens);

	ExpressionNode condition;
	vector<StatementNode> statements;
	vector<StatementNode> elseStatements;
};

#endif