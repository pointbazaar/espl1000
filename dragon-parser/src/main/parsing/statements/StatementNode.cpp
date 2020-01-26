
//standard headers
#include <vector>

//project headers
#include "StatementNode.hpp"

StatementNode::StatementNode(TokenList tokens) throws Exception {

	TokenList copy = tokens.copy();

	if (copy.size() == 0) {
		throw new Exception("tried to parse a Statement, but there are no tokens left");
	}

	IToken first = copy.get(0);

	if (first instanceof LoopToken) {
		this.statementNode = new LoopStatementNode(copy);
	} else if (first instanceof WhileToken) {
		this.statementNode = new WhileStatementNode(copy);
	} else if (first instanceof IfToken) {
		this.statementNode = new IfStatementNode(copy);
	} else if (first instanceof ReturnToken) {
		this.statementNode = new ReturnStatementNode(copy);
	} else {
		//TODO: we have to figure something out here.
		//i don't want 'let' statements
		//because it just messes up the code and is
		//just bloat to write.
		//but parsing should be straightforward. to give good error messages

		IStatementNode statementNode1; //temp variable so that this.statementNode can be final
		try {
			final TokenList copy2 = copy.copy();
			statementNode1 = new MethodCallNode(copy2);
			copy2.expectAndConsumeOtherWiseThrowException(new SemicolonToken());

			copy.set(copy2);
		} catch (Exception e1) {
			statementNode1 = new AssignmentStatementNode(copy);
		}
		this.statementNode = statementNode1;
	}

	tokens.set(copy);
}

