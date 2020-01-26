#ifndef ASSIGNMENTSTATEMENTNODE
#define 


//standard headers
#include <vector>
#include <map>
#include <algorithm>

//project headers
#include "AssignmentStatementNode.hpp"

class AssignmentStatementNode : IASTNode, IStatementNode {

public:
	Optional<TypeNode> optTypeNode;

	VariableNode variableNode;

	ExpressionNode expressionNode;

	AssignmentStatementNode(TokenList tokens) throws Exception;
};