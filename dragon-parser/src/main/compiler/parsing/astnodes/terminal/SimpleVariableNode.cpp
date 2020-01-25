
//standard headers
#include <vector>
#include <set>
#include <optional>
#include <string>

//project headers
#include "SimpleVariableNode.hpp"
#include "../nonterminal/ExpressionNode.hpp"

using namespace std;

class SimpleVariableNode {

public:
	string name;
	optional<ExpressionNode> indexOptional;

	SimpleVariableNode(TokenList tokens) {

		TokenList copy = new TokenList(tokens);

		IToken token = copy.get(0);

		if (token instanceof IdentifierToken) {
			this.name = token.getContents();
			copy.consume(1);

			//it could have an index
			if (copy.size() > 0 && copy.get(0) instanceof LBracketToken) {
				copy.expectAndConsumeOtherWiseThrowException(new LBracketToken());
				this.indexOptional = Optional.of(new ExpressionNode(copy));
				copy.expectAndConsumeOtherWiseThrowException(new RBracketToken());
			} else {
				this.indexOptional = Optional.empty();
				//pass, this assignment has no index to it
			}

		} else {
			String msg = tokens.relPath.toString() + ":" + token.getLineNumber();
			msg += ": could not read variable name. token was " + token.getContents();
			msg += " from context  '" + tokens.toSourceCodeFragment().substring(0, Math.min(20, tokens.toSourceCodeFragment().length())) + "'";

			throw new Exception(msg);
		}

		tokens.set(copy);
	}

};