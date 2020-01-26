
//standard headers
#include <vector>
#include <set>

//project headers
#include "ArrayConstantNode.hpp"

using namespace std;

ArrayConstantNode::ArrayConstantNode(TokenList tokens){

	TokenList copy = new TokenList(tokens);

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

