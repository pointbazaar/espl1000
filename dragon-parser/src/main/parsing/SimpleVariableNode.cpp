
//standard headers
#include <vector>
#include <set>
#include <optional>
#include <string>

//project headers
#include "SimpleVariableNode.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

SimpleVariableNode::SimpleVariableNode(TokenList tokens) {

	TokenList copy = TokenList(tokens);

	BaseToken token = copy.get(0);

	if (token.kind == IDENTIFIER) {
		this->name = token.getContents();
		copy.consume(1);

		//it could have an index
		if (copy.size() > 0 && copy.get(0).kind == LBRACKET) {
			copy.expectAndConsumeOtherWiseThrowException(BaseToken(LBRACKET));
			this->indexOptional = optional(ExpressionNode(copy));
			copy.expectAndConsumeOtherWiseThrowException(BaseToken(RBRACKET));
		} else {
			this->indexOptional = nullopt;
			//pass, this assignment has no index to it
		}

	} else {
		string msg = tokens.relPath.toString() + ":" + token.getLineNumber();
		msg += ": could not read variable name. token was " + token.getContents();
		msg += " from context  '" + tokens.toSourceCodeFragment().substr(0, min(20, tokens.toSourceCodeFragment().size())) + "'";

		throw msg;
	}

	tokens.set(copy);
}

