
//standard headers
#include <vector>
#include <set>

//project headers
#include "SimpleTypeNode.hpp"
#include "../../lexing/TokenList.hpp"
#include "../../lexing/BaseToken.hpp"
#include "../../commandline/TokenKeys.hpp"

using namespace std;

SimpleTypeNode::SimpleTypeNode(TokenList tokens) {

	if (tokens.size() == 0) {
		throw "exception";
	}

	BaseToken token = tokens.get(0);
	if (token.kind == TYPEIDENTIFIER) {
		this->typeName = token.getContents();
		tokens.consume(1);
	} else if (token.kind == ANYTYPE) {
		tokens.consume(1);
		this->typeName = "#";
	} else {
		throw "could not read simple type identifier";
	}
}

SimpleTypeNode::SimpleTypeNode(string typeName) {
	this->typeName = typeName;
}


