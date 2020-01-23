
//standard headers
#include <vector>
#include <set>

//project headers
#include "ArrayTypeNode.hpp"

public final class ArrayTypeNode implements IASTNode, ITypeNode {

	public final TypeNode element_type;

	public ArrayTypeNode(final TypeNode element_type) {
		this.element_type = element_type;
	}

	public ArrayTypeNode(final TokenList tokens) throws Exception {

		final TokenList copy1 = tokens.copy();

		copy1.expectAndConsumeOtherWiseThrowException(new LBracketToken());
		this.element_type = new TypeNode(copy1);
		copy1.expectAndConsumeOtherWiseThrowException(new RBracketToken());


		tokens.set(copy1);
	}

}
