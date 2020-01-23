
//standard headers
#include <vector>

//project headers
#include "AssignmentStatementNode.hpp"

public final class AssignmentStatementNode implements IASTNode, IStatementNode {

	public final Optional<TypeNode> optTypeNode;

	public final VariableNode variableNode;

	public final ExpressionNode expressionNode;

	public AssignmentStatementNode(final TokenList tokens) throws Exception {
		Optional<TypeNode> optTypeNode1; //to have this.optTypeNode as final

		final TokenList copy = tokens.copy();

		try {
			final TokenList copy2 = copy.copy();
			optTypeNode1 = Optional.of(new TypeNode(copy2));
			copy.set(copy2);
		} catch (Exception e) {
			optTypeNode1 = Optional.empty();
			//pass
		}

		optTypeNode = optTypeNode1;
		this.variableNode = new VariableNode(copy);

		copy.expectAndConsumeOtherWiseThrowException(new OperatorToken("="));

		this.expressionNode = new ExpressionNode(copy);

		copy.expectAndConsumeOtherWiseThrowException(new SemicolonToken());

		tokens.set(copy);
	}

}
