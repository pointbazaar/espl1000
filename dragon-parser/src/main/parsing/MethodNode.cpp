//standard headers
#include <vector>
#include <set>

//project headers
#include "MethodNode.hpp"
#include "../lexing/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

MethodNode::MethodNode(TokenList tokens, bool debug) {

	if (debug) {
		System.out.println("try to parse MethodNode from '" + tokens.toSourceCodeFragment() + "'");
	}

	TokenList copy = tokens.copy();

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(FN));

	this->methodName = IdentifierNode(copy).identifier;

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(LPARENS));

	//while there is no ')' up, continue parsing arguments
	BaseToken next = copy.get(0);
	while (!(next instanceof RParensToken)) {
		if (this->arguments.size() > 0) {
			copy.expectAndConsumeOtherWiseThrowException(BaseToken(COMMA));
		}
		this->arguments.add(DeclaredArgumentNode(copy));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RPARENS));

	if (copy.head() instanceof ArrowToken) {
		BaseToken head = copy.head();
		this->hasSideEffects = false;	//TODO: put in the real value
		copy.consume(1);
	} else {
		throw ("expected arrow here");
	}

	this->returnType = TypeNode(copy);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(LCURLY));

	IToken statement_next = copy.get(0);
	while (!(statement_next.kind != RCURLY)) {
		this->statements.push_back(StatementNode(copy));
		statement_next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RCURLY));

	tokens.set(copy);
}

