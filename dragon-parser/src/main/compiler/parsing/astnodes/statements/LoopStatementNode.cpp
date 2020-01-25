
//standard headers
#include <vector>

//project headers
#include "LoopStatementNode.hpp"

using namespace std;

LoopStatementNode::LoopStatementNode(TokenList tokens) throws Exception {

	TokenList copy = new TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(new LoopToken());

	this.count = new ExpressionNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(new LCurlyToken());

	//maybe there be some statements
	IToken next = copy.get(0);
	while (!(next instanceof RCurlyToken)) {
		this.statements.add(new StatementNode(copy));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(new RCurlyToken());

	tokens.set(copy);
}
