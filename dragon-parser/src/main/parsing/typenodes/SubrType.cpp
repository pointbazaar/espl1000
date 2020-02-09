#include <vector>
#include <iostream>

#include "SubrType.hpp"
#include "Type.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"

SubrType::SubrType(Type* return_type, bool hasSideEffects){
	this->returnType = return_type;
	this->hasSideEffects = hasSideEffects;
}

SubrType::SubrType(TokenList tokens){

	TokenList copy = tokens.copy();

	copy.expect(Token(LPARENS));

	bool sucess_argument_types = true;
	try {
		this->argumentTypes.push_back(new Type(copy));
	} catch (std::string e) {
		sucess_argument_types = false;
	}
	while (sucess_argument_types) {
		try {
			TokenList copy2 = copy.copy();

			copy2.expect(Token(COMMA));
			this->argumentTypes.push_back(new Type(copy2));

			copy.set(copy2);
		} catch (std::string e) {
			sucess_argument_types = false;
		}
	}

	copy.expect(Token(RPARENS));

	if (copy.head().kind == ARROW) {
		Token arrow = copy.head();
		this->hasSideEffects = false;//!arrow.is_functional;
		copy.consume(1);
	} else {
		throw "expected an arrow token here";
	}

	this->returnType = new Type(copy);

	tokens.set(copy);
}

