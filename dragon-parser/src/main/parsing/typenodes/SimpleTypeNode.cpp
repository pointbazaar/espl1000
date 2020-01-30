
//standard headers
#include <vector>
#include <set>

//project headers
#include "SimpleTypeNode.hpp"
#include "TokenList.hpp"


SimpleTypeNode::SimpleTypeNode(TokenList tokens) {
	if (tokens.size() == 0) {
		throw new Exception();
	}

	IToken token = tokens.get(0);
	if (token instanceof TypeIdentifierToken) {
		this.typeName =
				token.getContents();

		tokens.consume(1);
	} else if (token instanceof AnyTypeToken) {
		tokens.consume(1);
		this.typeName = "#";
	} else {
		throw "could not read simple type identifier";
	}
}

SimpleTypeNode::SimpleTypeNode(string typeName) {
	this.typeName = typeName;
}


