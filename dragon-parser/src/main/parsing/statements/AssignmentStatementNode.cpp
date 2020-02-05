
//standard headers
#include <vector>
#include <map>
#include <algorithm>
#include <optional>

//project headers
#include "AssignmentStatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/BaseToken.hpp"
#include "../../commandline/TokenKeys.hpp"

AssignmentStatementNode::AssignmentStatementNode(TokenList tokens) {
	optional<TypeNode> optTypeNode1; 

	TokenList copy = tokens.copy();

	try {
		TokenList copy2 = copy.copy();
		optTypeNode1 = optional<TypeNode>(TypeNode(copy2));
		copy.set(copy2);
	} catch (string e) {
		optTypeNode1 = nullopt;
		//pass
	}

	optTypeNode = optTypeNode1;
	this->variableNode = VariableNode(copy);

	copy.expectAndConsumeOtherWiseThrowException( BaseToken(OPERATOR,"="));

	this->expressionNode = ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException( BaseToken(SEMICOLON));

	tokens.set(copy);
}

