
//standard headers
#include <vector>

//project headers
#include "MethodCallNode.hpp"

public final class MethodCallNode implements IASTNode, IStatementNode, ITermNode {

	public final String methodName;

	public final List<ExpressionNode> arguments = new ArrayList<>();

	public MethodCallNode(final TokenList tokens) throws Exception {

		final TokenList copy = tokens.copy();

		this.methodName = new IdentifierNode(copy).identifier;

		copy.expectAndConsumeOtherWiseThrowException(new LParensToken());

		//while there is no ')' up, continue parsing arguments
		IToken next = copy.get(0);
		while (!(next instanceof RParensToken)) {
			if (arguments.size() > 0) {
				copy.expectAndConsumeOtherWiseThrowException(new CommaToken());
			}
			this.arguments.add(new ExpressionNode(copy));
			next = copy.get(0);
		}

		copy.expectAndConsumeOtherWiseThrowException(new RParensToken());

		tokens.set(copy);
	}

}
