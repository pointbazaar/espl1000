
//standard headers
#include <vector>

//project headers
#include "ReturnStatementNode.hpp"

using namespace std;

ReturnStatemenNode::ReturnStatementNode(TokenList tokens) throws Exception {

	TokenList copy = new TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(new ReturnToken());

	this.returnValue = new ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(new SemicolonToken());

	tokens.set(copy);
}

