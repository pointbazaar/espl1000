
//standard headers
#include <vector>
#include <set>

//project headers
#include "CharConstNode.hpp"

using namespace std;

class CharConstNode : IASTNode, ITermNode {

public:
	char content;

	CharConstNode(TokenList tokens) throws Exception {

		IToken token = tokens.get(0);

		if (token instanceof CharConstantToken) {
			this.content = ((CharConstantToken) token).content;
			tokens.consume(1);
		} else {
			throw new Exception("could not read charConstant node");
		}

	}

};