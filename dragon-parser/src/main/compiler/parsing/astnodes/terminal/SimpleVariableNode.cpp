
//standard headers
#include <vector>
#include <set>

//project headers
#include "SimpleVariableNode.hpp"

public final class SimpleVariableNode implements IASTNode {

	//this identifies a variable

	public final String name;

	public final Optional<ExpressionNode> indexOptional;

	public SimpleVariableNode(final TokenList tokens) throws Exception {

		final TokenList copy = new TokenList(tokens);

		final IToken token = copy.get(0);

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

}
