
//standard headers
#include <vector>
#include <set>

//project headers
#include "CharConstNode.hpp"

public final class CharConstNode implements IASTNode, ITermNode {

	public final char content;

	public CharConstNode(final TokenList tokens) throws Exception {

		final IToken token = tokens.get(0);

		if (token instanceof CharConstantToken) {
			this.content = ((CharConstantToken) token).content;
			tokens.consume(1);
		} else {
			throw new Exception("could not read charConstant node");
		}

	}

}
