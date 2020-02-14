#include <vector>
#include <iostream>

#include "BasicTypeWrapped.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "SubrType.hpp"
#include "SimpleType.hpp"

using namespace std;

struct BasicTypeWrapped* makeBasicTypeWrapped(struct SimpleType* typeNode) {

	struct BasicTypeWrapped* res = (struct BasicTypeWrapped*)malloc(sizeof(struct BasicTypeWrapped));

	res->m1 = typeNode;
	res->m2 = NULL;

	return res;
}

struct BasicTypeWrapped* makeBasicTypeWrapped(struct SubrType* typeNode) {

	struct BasicTypeWrapped* res = (struct BasicTypeWrapped*)malloc(sizeof(struct BasicTypeWrapped));

	res->m1 = NULL;
	res->m2 = typeNode;

	return res;
}

struct BasicTypeWrapped* makeBasicTypeWrapped(TokenList* tokens, bool debug) {

	if(debug){
		cout << "BasicTypeWrapped(...)" << endl;
	}

	struct BasicTypeWrapped* res = (struct BasicTypeWrapped*)malloc(sizeof(struct BasicTypeWrapped));

	TokenList copy = tokens->copy();

	if (copy.size() > 1 && copy.get(0).tokenEquals(LPARENS)) {
		TokenList copy2 = copy.copy();

		copy2.expect(LPARENS);
		res->m2 = makeSubrType(&copy2,debug);
		copy2.expect(RPARENS);

		copy.set(copy2);

	} else {
		res->m1 = makeSimpleType(&copy,debug);
	}

	tokens->set(copy);

	return res;
}

