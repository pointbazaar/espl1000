
//standard headers
#include <vector>
#include <map>
#include <algorithm>
#include <optional>

//project headers
#include "AssignmentStatementNode.hpp"


AssignmentStatementNode(TokenList tokens) throws Exception {
	optional<TypeNode> optTypeNode1; //to have this.optTypeNode as final

	TokenList copy = tokens.copy();

	try {
		TokenList copy2 = copy.copy();
		optTypeNode1 = Optional.of(new TypeNode(copy2));
		copy.set(copy2);
	} catch (Exception e) {
		optTypeNode1 = Optional.empty();
		//pass
	}

	optTypeNode = optTypeNode1;
	this.variableNode = new VariableNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(new OperatorToken("="));

	this.expressionNode = new ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(new SemicolonToken());

	tokens.set(copy);
}

