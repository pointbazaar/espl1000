#ifndef STATEMENTNODE
#define STATEMENTNODE

#include <variant>

#include "../../commandline/TokenList.hpp"
#include "MethodCallNode.hpp"
#include "WhileStatementNode.hpp"
#include "IfStatementNode.hpp"
#include "ReturnStatementNode.hpp"
#include "AssignmentStatementNode.hpp"

using namespace std;

class StatementNode {

public:
	StatementNode(TokenList tokens);
	variant<MethodCallNode,WhileStatementNode,IfStatementNode,ReturnStatementNode,AssignmentStatementNode> statementNode;
};

#endif