#include <vector>

#include "ArrayType.hpp"
#include "Type.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"

ArrayType::ArrayType(Type* element_type){
	this->element_type = element_type;
}

ArrayType::ArrayType(TokenList tokens) {

	TokenList copy1 = tokens.copy();

	copy1.expect(Token(LBRACKET));
	this->element_type = new Type(copy1);
	copy1.expect(Token(RBRACKET));

	tokens.set(copy1);
}

