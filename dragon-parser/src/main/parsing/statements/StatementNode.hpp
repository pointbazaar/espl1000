#ifndef STATEMENTNODE
#define STATEMENTNODE

#include "IStatementNode.hpp"
#include "../../commandline/TokenList.hpp"

using namespace std;

class StatementNode {

public:
	StatementNode(TokenList tokens);
	IStatementNode statementNode;
};

#endif