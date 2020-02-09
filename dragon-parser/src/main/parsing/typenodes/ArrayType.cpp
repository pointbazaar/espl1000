#include <vector>
#include <iostream>

#include "ArrayType.hpp"
#include "Type.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"

using namespace std;

ArrayType::ArrayType(Type* element_type){
	this->element_type = element_type;
}

ArrayType::ArrayType(TokenList tokens, bool debug) {

	if(debug){
		cout << "ArrayType(...)" << endl;
	}

	TokenList copy1 = tokens.copy();

	copy1.expect(Token(LBRACKET));
	this->element_type = new Type(copy1,debug);
	copy1.expect(Token(RBRACKET));

	tokens.set(copy1);
}

