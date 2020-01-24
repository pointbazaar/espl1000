
//standard headers
#include <vector>
#include <set>

//project headers
#include "BoolConstNode.hpp"

class BoolConstNode : IASTNode, ITermNode {

public:
	bool boolValue;

	BoolConstNode(TokenList tokens) throws Exception {

		TokenList copy = new TokenList(tokens);

		if (copy.get(0) instanceof BoolConstantToken) {
			BoolConstantToken tk = (BoolConstantToken) copy.get(0);
			this.boolValue = tk.value;
			copy.consume(1);
		} else {
			throw new Exception("could not read Bool Constant node");
		}

		tokens.set(copy);
	}

};