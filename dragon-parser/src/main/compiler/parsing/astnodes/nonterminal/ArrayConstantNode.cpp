
//standard headers
#include <vector>
#include <set>

//project headers
#include "ArrayConstantNode.hpp"

public final class ArrayConstantNode implements IASTNode, ITermNode {

	public List<ExpressionNode> elements = new ArrayList<>();

	public ArrayConstantNode(final TokenList tokens) throws Exception {

		final TokenList copy = new TokenList(tokens);

		copy.expectAndConsumeOtherWiseThrowException(new LBracketToken());

		IToken next = copy.get(0);
		while (!(next instanceof RBracketToken)) {
			if (this.elements.size() > 0) {
				copy.expectAndConsumeOtherWiseThrowException(new CommaToken());
			}
			this.elements.add(new ExpressionNode(copy));
			next = copy.get(0);
		}

		copy.expectAndConsumeOtherWiseThrowException(new RBracketToken());

		tokens.set(copy);
	}

}
