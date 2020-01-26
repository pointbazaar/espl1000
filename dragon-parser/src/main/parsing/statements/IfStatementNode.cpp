
//standard headers
#include <vector>
#include <string>
#include <map>
#include <stdio.h>

//project headers
#include "IfStatementNode.hpp"

using namespace std;

IfStatementNode::IfStatementNode(TokenList tokens) throws Exception {

	TokenList copy = new TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(new IfToken());

	copy.expectAndConsumeOtherWiseThrowException(new LParensToken());

	this.condition = new ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(new RParensToken());

	copy.expectAndConsumeOtherWiseThrowException(new LCurlyToken());

	IToken next = copy.get(0);

	while (!(next instanceof RCurlyToken)) {

		this.statements.add(new StatementNode(copy));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(new RCurlyToken());

	//maybe there is an else
	if (copy.startsWith(new ElseToken())) {

		copy.expectAndConsumeOtherWiseThrowException(new ElseToken());

		copy.expectAndConsumeOtherWiseThrowException(new LCurlyToken());

		//maybe there be some statements
		IToken elsenext = copy.get(0);

		while (!(elsenext instanceof RCurlyToken)) {

			this.elseStatements.add(new StatementNode(copy));
			elsenext = copy.get(0);
		}

		copy.expectAndConsumeOtherWiseThrowException(new RCurlyToken());
	}

	tokens.set(copy);
}

