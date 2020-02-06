
//standard headers
#include <vector>

//project headers
#include "WhileStatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/BaseToken.hpp"
#include "../ExpressionNode.hpp"
#include "StatementNode.hpp"

using namespace std;

WhileStatementNode::WhileStatementNode(TokenList tokens){

	TokenList copy = TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(WHILE));

	this->condition = new ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(LCURLY));

	BaseToken next = copy.get(0);
	while (!(next.kind == RCURLY)) {
		this->statements.push_back(new StatementNode(copy));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RCURLY));

	tokens.set(copy);
}

