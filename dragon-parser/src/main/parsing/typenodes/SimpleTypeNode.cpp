
#include <vector>
#include "SimpleTypeNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"

SimpleTypeNode::SimpleTypeNode(TokenList tokens) {

	if (tokens.size() == 0) {
		throw "exception";
	}

	Token token = tokens.get(0);
	if (token.kind == TYPEIDENTIFIER) {
		this->typeName = token.value;
	} else if (token.kind == ANYTYPE) {
		this->typeName = "#";
	} else {
		throw "could not read simple type identifier";
	}
	tokens.consume(1);
}

SimpleTypeNode::SimpleTypeNode(std::string typeName) {
	this->typeName = typeName;
}


