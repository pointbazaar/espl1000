#ifndef WHILESTATEMENTNODE
#define WHILESTATEMENTNODE

//standard headers
#include <vector>

//project headers
#include "../ExpressionNode.hpp"
#include "StatementNode.hpp"
#include "../../commandline/TokenList.hpp"

using namespace std;

class WhileStatementNode  {

public:
	WhileStatementNode(TokenList tokens);

	ExpressionNode condition;
	vector<StatementNode> statements;
};

#endif