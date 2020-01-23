
//standard headers
#include <vector>
#include <set>

//project headers
#include "BoolConstNode.hpp"

public final class BoolConstNode implements IASTNode, ITermNode {

	public final boolean boolValue;

	public BoolConstNode(final TokenList tokens) throws Exception {

		final TokenList copy = new TokenList(tokens);

		if (copy.get(0) instanceof BoolConstantToken) {
			BoolConstantToken tk = (BoolConstantToken) copy.get(0);
			this.boolValue = tk.value;
			copy.consume(1);
		} else {
			throw new Exception("could not read Bool Constant node");
		}

		tokens.set(copy);
	}

}
