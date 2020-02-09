//standard headers
#include <vector>
#include <iostream>
#include "MethodNode.hpp"
#include "typenodes/TypeNode.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/BaseToken.hpp"
#include "IdentifierNode.hpp"
#include "statements/StatementNode.hpp"
#include "DeclaredArgumentNode.hpp"

using namespace std;

MethodNode::MethodNode(TokenList tokens, bool debug) {

	if (debug) {
		cout << "MethodNode(...)" << endl;
		cout << "from: " << endl << tokens.toSourceCodeFragment() << "" << endl;
	}

	TokenList copy = tokens.copy();

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(FN));

	this->methodName = IdentifierNode(&copy,debug).identifier;

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(LPARENS));

	//while there is no ')' up, continue parsing arguments
	BaseToken next = copy.get(0);
	while (!(next.kind == RPARENS)) {
		if (this->arguments.size() > 0) {
			copy.expectAndConsumeOtherWiseThrowException(BaseToken(COMMA));
		}
		this->arguments.push_back(new DeclaredArgumentNode(copy, debug));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RPARENS));

	if (copy.head().kind == ARROW) {
		BaseToken head = copy.head();
		this->hasSideEffects = false;	//TODO: put in the real value
		copy.consume(1);
	} else {
		throw "expected arrow here";
	}

	this->returnType = new TypeNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(LCURLY));

	BaseToken statement_next = copy.get(0);
	while (!(statement_next.kind != RCURLY)) {
		this->statements.push_back(new StatementNode(copy,debug));
		statement_next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RCURLY));

	tokens.set(copy);
}

