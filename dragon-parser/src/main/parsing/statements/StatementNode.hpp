#ifndef STATEMENTNODE
#define STATEMENTNODE

#include "IStatementNode.hpp"
#include "../../commandline/TokenList.hpp"

class StatementNode {

public:
	StatementNode(TokenList tokens);
	IStatementNode statementNode;
};

#endif