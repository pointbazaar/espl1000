
//standard headers
#include <vector>
#include <iostream>
//project headers
#include "MethodCallNode.hpp"
#include "../IdentifierNode.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/BaseToken.hpp"
#include "../ExpressionNode.hpp"

MethodCallNode::MethodCallNode(TokenList tokens,bool debug) {

	TokenList copy = tokens.copy();

	this->methodName = IdentifierNode(copy).identifier;

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(LPARENS));

	//while there is no ')' up, continue parsing arguments
	BaseToken next = copy.get(0);
	while (!(next.kind == RPARENS)) {
		if (arguments.size() > 0) {
			copy.expectAndConsumeOtherWiseThrowException(BaseToken(COMMA));
		}
		this->arguments.push_back(new ExpressionNode(copy));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RPARENS));

	tokens.set(copy);
}

