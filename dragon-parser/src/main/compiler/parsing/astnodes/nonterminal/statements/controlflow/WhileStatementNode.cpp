
//standard headers
#include <vector>

//project headers
#include "WhileStatementNode.hpp"


WhileStatementNode::WhileStatementNode(final TokenList tokens) throws Exception {

	TokenList copy = new TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(new WhileToken());

	copy.expectAndConsumeOtherWiseThrowException(new LParensToken());

	this.condition = new ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(new RParensToken());

	copy.expectAndConsumeOtherWiseThrowException(new LCurlyToken());

	BaseToken next = copy.get(0);
	while (!(next instanceof RCurlyToken)) {
		this.statements.add(new StatementNode(copy));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(new RCurlyToken());

	tokens.set(copy);
}

