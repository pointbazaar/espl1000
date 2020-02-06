#ifndef ASSIGNMENTSTATEMENTNODE
#define ASSIGNMENTSTATEMENTNODE

//standard headers
#include <vector>
#include <map>
#include <algorithm>
#include <optional>

//project headers
#include "AssignmentStatementNode.hpp"
#include "../VariableNode.hpp"
#include "../ExpressionNode.hpp"
#include "../typenodes/TypeNode.hpp"
#include "../../commandline/TokenList.hpp"

class AssignmentStatementNode {

public:
	AssignmentStatementNode(TokenList tokens);
	
	optional<TypeNode> optTypeNode;
	VariableNode variableNode;
	ExpressionNode expressionNode;
};

#endif