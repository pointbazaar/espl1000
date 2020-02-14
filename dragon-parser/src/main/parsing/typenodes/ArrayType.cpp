#include <vector>
#include <iostream>

#include "ArrayType.hpp"
#include "Type.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"

using namespace std;

struct ArrayType* makeArrayType(struct Type* element_type){
	struct ArrayType* res = (struct ArrayType*)malloc(sizeof(struct ArrayType));

	res->element_type = element_type;

	return res;
}

struct ArrayType* makeArrayType(TokenList* tokens, bool debug) {

	if(debug){
		cout << "ArrayType(...)" << endl;
	}

	struct ArrayType* res = (struct ArrayType*)malloc(sizeof(struct ArrayType));

	TokenList copy1 = tokens->copy();

	copy1.expect(LBRACKET);
	res->element_type = makeType(&copy1,debug);
	copy1.expect(RBRACKET);

	tokens->set(copy1);

	return res;
}

