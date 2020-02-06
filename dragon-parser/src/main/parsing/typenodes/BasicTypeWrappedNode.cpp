
//standard headers
#include <vector>
#include <set>
#include <variant>

//project headers
#include "BasicTypeWrappedNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "SubroutineTypeNode.hpp"
#include "SimpleTypeNode.hpp"

//using namespace std;

BasicTypeWrappedNode::BasicTypeWrappedNode(SimpleTypeNode* typeNode) {
	this->m1 = typeNode;
}

BasicTypeWrappedNode::BasicTypeWrappedNode(SubroutineTypeNode* typeNode) {
	this->m2 = typeNode;
}

BasicTypeWrappedNode::BasicTypeWrappedNode(TokenList tokens) {

	TokenList copy = tokens.copy();

	if (copy.size() > 1 && copy.get(0).tokenEquals(BaseToken(LPARENS))) {
		TokenList copy2 = copy.copy();

		copy2.expectAndConsumeOtherWiseThrowException(BaseToken(LPARENS));
		this->m2 = new SubroutineTypeNode(copy2);
		copy2.expectAndConsumeOtherWiseThrowException(BaseToken(RPARENS));

		copy.set(copy2);

	} else {
		this->m1 = new SimpleTypeNode(copy);
	}
	tokens.set(copy);
}

