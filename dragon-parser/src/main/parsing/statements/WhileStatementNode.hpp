#ifndef WHILESTATEMENTNODE
#define WHILESTATEMENTNODE

#include <vector>

//project headers
class ExpressionNode;
class StatementNode;
#include "../../commandline/TokenList.hpp"

class WhileStatementNode  {

public:
	WhileStatementNode(TokenList tokens,bool debug);

	ExpressionNode* condition;
	vector<StatementNode*> statements;
};

#endif