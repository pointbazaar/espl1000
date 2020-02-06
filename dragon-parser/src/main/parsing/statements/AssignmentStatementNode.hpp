#ifndef ASSIGNMENTSTATEMENTNODE
#define ASSIGNMENTSTATEMENTNODE

//standard headers
#include <optional>

//project headers
#include "AssignmentStatementNode.hpp"
class VariableNode;
class ExpressionNode;
class TypeNode;
#include "../../commandline/TokenList.hpp"

class AssignmentStatementNode {

public:
	AssignmentStatementNode(TokenList tokens);
	
	optional<TypeNode*> optTypeNode;
	VariableNode* variableNode;
	ExpressionNode* expressionNode;
};

#endif