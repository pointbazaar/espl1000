#include <vector>
#include <iostream>

#include "SimpleType.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"

using namespace std;

struct SimpleType* makeSimpleType(TokenList* tokens, bool debug) {

	if(debug){
		cout << "SimpleType(...)" << endl;
	}

	struct SimpleType* res = (struct SimpleType*)malloc(sizeof(struct SimpleType));

	Token token = tokens->get(0);
	if (token.kind == TYPEIDENTIFIER) {
		res->typeName = (char*)token.value.c_str();
	} else if (token.kind == ANYTYPE) {
		res->typeName = (char*)string("#").c_str();
	} else {
		cout << "Error: could not read simple type identifier" << endl;
		throw string("could not read simple type identifier");
	}
	tokens->consume(1);

	return res;
}

struct SimpleType* makeSimpleType(char* typeName) {
	struct SimpleType* res = (struct SimpleType*)malloc(sizeof(struct SimpleType));

	res->typeName = typeName;

	return res;
}
