
//standard headers
#include <vector>
#include <set>

//project headers
#include "BasicTypeWrappedNode.hpp"

class BasicTypeWrappedNode :  ITypeNode {

public:
	IBasicAndWrappedTypeNode typeNode;

	BasicTypeWrappedNode(IBasicAndWrappedTypeNode typeNode) {
		this.typeNode = typeNode;
	}

	BasicTypeWrappedNode(TokenList tokens) {

		TokenList copy = tokens.copy();

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

};