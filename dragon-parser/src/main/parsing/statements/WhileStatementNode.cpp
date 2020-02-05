
//standard headers
#include <vector>
#include <map>
#include <algorithm>

//project headers
#include "WhileStatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/BaseToken.hpp"

using namespace std;

WhileStatementNode::WhileStatementNode(TokenList tokens){

	TokenList copy = TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(WHILE));

	this->condition = ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(LCURLY));

	BaseToken next = copy.get(0);
	while (!(next.kind == RCURLY)) {
		this->statements.push_back(StatementNode(copy));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RCURLY));

	tokens.set(copy);
}

