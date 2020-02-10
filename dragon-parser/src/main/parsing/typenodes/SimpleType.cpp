#include <vector>
#include <iostream>

#include "SimpleType.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"

using namespace std;

SimpleType::SimpleType(TokenList tokens, bool debug) {

	if(debug){
		cout << "SimpleType(...)" << endl;
	}

	Token token = tokens.get(0);
	if (token.kind == TYPEIDENTIFIER) {
		this->typeName = token.value;
	} else if (token.kind == ANYTYPE) {
		this->typeName = "#";
	} else {
		cout << "Error: could not read simple type identifier" << endl;
		throw string("could not read simple type identifier");
	}
	tokens.consume(1);
}

SimpleType::SimpleType(std::string typeName) {
	this->typeName = typeName;
}
