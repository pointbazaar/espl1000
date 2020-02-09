#include <vector>

#include "SimpleType.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"

SimpleType::SimpleType(TokenList tokens) {

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

SimpleType::SimpleType(std::string typeName) {
	this->typeName = typeName;
}


