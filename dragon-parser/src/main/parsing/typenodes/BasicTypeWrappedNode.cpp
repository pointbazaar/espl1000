
//standard headers
#include <vector>
#include <set>

//project headers
#include "BasicTypeWrappedNode.hpp"
#include "../../lexing/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "SubroutineTypeNode.hpp"

using namespace std;

BasicTypeWrappedNode::BasicTypeWrappedNode(IBasicAndWrappedTypeNode typeNode) {
	this->typeNode = typeNode;
}

BasicTypeWrappedNode::BasicTypeWrappedNode(TokenList tokens) {

	TokenList copy = tokens.copy();

	IBasicAndWrappedTypeNode candidate;
	if (copy.size() > 1 && copy.get(0).tokenEquals(BaseToken(LPARENS))) {
		TokenList copy2 = copy.copy();

		copy2.expectAndConsumeOtherWiseThrowException(BaseToken(LPARENS));
		candidate = SubroutineTypeNode(copy2);
		copy2.expectAndConsumeOtherWiseThrowException(BaseToken(RPARENS));

		copy.set(copy2);

	} else {
		candidate = SimpleTypeNode(copy);
	}
	this->typeNode = candidate;
	tokens.set(copy);
}

