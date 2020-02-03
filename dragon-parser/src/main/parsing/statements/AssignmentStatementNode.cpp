
//standard headers
#include <vector>
#include <map>
#include <algorithm>
#include <optional>

//project headers
#include "AssignmentStatementNode.hpp"


AssignmentStatementNode(TokenList tokens) {
	optional<TypeNode> optTypeNode1; 

	TokenList copy = tokens.copy();

	try {
		TokenList copy2 = copy.copy();
		optTypeNode1 = optional<TypeNode>(TypeNode(copy2));
		copy.set(copy2);
	} catch (string e) {
		optTypeNode1 = Optional.empty();
		//pass
	}

	optTypeNode = optTypeNode1;
	this->variableNode = VariableNode(copy);

	copy.expectAndConsumeOtherWiseThrowException( OperatorToken("="));

	this->expressionNode = ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException( SemicolonToken());

	tokens.set(copy);
}

