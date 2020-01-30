
//standard headers
#include <vector>
#include <set>

//project headers
#include "SimpleTypeNode.hpp"
#include "../../lexing/TokenList.hpp"
#include "../../lexing/BaseToken.hpp"

using namespace std;

SimpleTypeNode::SimpleTypeNode(TokenList tokens) {
	if (tokens.size() == 0) {
		throw new Exception();
	}

	BaseToken token = tokens.get(0);
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


