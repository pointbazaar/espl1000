
//standard headers
#include <vector>
#include <set>

//project headers
#include "CharConstNode.hpp"
#include "../lexing/TokenList.hpp"

using namespace std;

CharConstNode::CharConstNode(TokenList tokens) {

	BaseToken token = tokens.get(0);

	if (token instanceof CharConstantToken) {
		this.content = ((CharConstantToken) token).content;
		tokens.consume(1);
	} else {
		throw new Exception("could not read charConstant node");
	}

}

