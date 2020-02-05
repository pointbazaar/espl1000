
//standard headers
#include <vector>
#include <string>
#include <map>
#include <stdio.h>

//project headers
#include "IfStatementNode.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/TokenList.hpp"

using namespace std;

IfStatementNode::IfStatementNode(TokenList tokens) {

	TokenList copy = TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(IF));

	this->condition = ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(LCURLY));

	BaseToken next = copy.get(0);

	while (!(next.kind == RCURLY)) {

		this->statements.push_back(StatementNode(copy));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RCURLY));

	//maybe there is an else
	if (copy.startsWith(BaseToken(ELSE))) {

		copy.expectAndConsumeOtherWiseThrowException(BaseToken(ELSE));

		copy.expectAndConsumeOtherWiseThrowException(BaseToken(LCURLY));

		//maybe there be some statements
		BaseToken elsenext = copy.get(0);

		while (!(elsenext.kind == RCURLY)) {

			this->elseStatements.push_back(StatementNode(copy));
			elsenext = copy.get(0);
		}

		copy.expectAndConsumeOtherWiseThrowException(BaseToken(RCURLY));
	}

	tokens.set(copy);
}

