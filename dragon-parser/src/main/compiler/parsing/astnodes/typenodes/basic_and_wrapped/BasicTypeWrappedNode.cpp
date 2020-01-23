
//standard headers
#include <vector>
#include <set>

//project headers
#include "BasicTypeWrappedNode.hpp"

public final class BasicTypeWrappedNode implements IASTNode, ITypeNode {

	public final IBasicAndWrappedTypeNode typeNode;

	public BasicTypeWrappedNode(final IBasicAndWrappedTypeNode typeNode) {
		this.typeNode = typeNode;
	}

	public BasicTypeWrappedNode(final TokenList tokens) throws Exception {

		final TokenList copy = tokens.copy();

		IBasicAndWrappedTypeNode candidate;
		if (copy.size() > 1 && copy.get(0).tokenEquals(new LParensToken())) {
			TokenList copy2 = copy.copy();

			copy2.expectAndConsumeOtherWiseThrowException(new LParensToken());
			candidate = new SubroutineTypeNode(copy2);
			copy2.expectAndConsumeOtherWiseThrowException(new RParensToken());

			copy.set(copy2);

		} else {
			candidate = new SimpleTypeNode(copy);
		}
		this.typeNode = candidate;
		tokens.set(copy);
	}

}
