
//standard headers
#include <vector>

//project headers
#include "ReturnStatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"

using namespace std;

ReturnStatementNode::ReturnStatementNode(TokenList tokens){

	TokenList copy = TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RETURN));

	this->returnValue = ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(SEMICOLON));

	tokens.set(copy);
}

