
#include "ReturnStatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/BaseToken.hpp"
#include "../ExpressionNode.hpp"

ReturnStatementNode::ReturnStatementNode(TokenList tokens){

	TokenList copy = TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RETURN));

	this->returnValue = new ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(SEMICOLON));

	tokens.set(copy);
}

