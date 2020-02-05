
//standard headers
#include <vector>

//project headers
#include "StatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/BaseToken.hpp"
#include "WhileStatementNode.hpp"
#include "IfStatementNode.hpp"
#include "ReturnStatementNode.hpp"
#include "MethodCallNode.hpp"
#include "AssignmentStatementNode.hpp"

StatementNode::StatementNode(TokenList tokens) {

	TokenList copy = tokens.copy();

	if (copy.size() == 0) {
		throw ("tried to parse a Statement, but there are no tokens left");
	}

	BaseToken first = copy.get(0);

	if (first.kind == LOOP) {
		//this->statementNode = LoopStatementNode(copy);
	} else if (first.kind == WHILE) {
		this->statementNode = WhileStatementNode(copy);
	} else if (first.kind == IF) {
		this->statementNode = IfStatementNode(copy);
	} else if (first.kind == RETURN) {
		this->statementNode = ReturnStatementNode(copy);
	} else {
		//TODO: we have to figure something out here.
		//i don't want 'let' statements
		//because it just messes up the code and is
		//just bloat to write.
		//but parsing should be straightforward. to give good error messages

		IStatementNode statementNode1; //temp variable so that this.statementNode can be final
		try {
			TokenList copy2 = copy.copy();
			statementNode1 = MethodCallNode(copy2);
			copy2.expectAndConsumeOtherWiseThrowException(BaseToken(SEMICOLON));

			copy.set(copy2);
		} catch (string e1) {
			statementNode1 = AssignmentStatementNode(copy);
		}
		this->statementNode = statementNode1;
	}

	tokens.set(copy);
}

