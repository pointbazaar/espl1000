
#include <vector>
#include <iostream>
#include "SubroutineTypeNode.hpp"
#include "TypeNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"

SubroutineTypeNode::SubroutineTypeNode(TypeNode* return_type, bool hasSideEffects){
	this->returnType = return_type;
	this->hasSideEffects = hasSideEffects;
}

SubroutineTypeNode::SubroutineTypeNode(TokenList tokens){

	TokenList copy = tokens.copy();

	copy.expectAndConsumeOtherWiseThrowException(Token(LPARENS));

	bool sucess_argument_types = true;
	try {
		this->argumentTypes.push_back(new TypeNode(copy));
	} catch (std::string e) {
		sucess_argument_types = false;
	}
	while (sucess_argument_types) {
		try {
			TokenList copy2 = copy.copy();

			copy2.expectAndConsumeOtherWiseThrowException(Token(COMMA));
			this->argumentTypes.push_back(new TypeNode(copy2));

			copy.set(copy2);
		} catch (std::string e) {
			sucess_argument_types = false;
		}
	}

	copy.expectAndConsumeOtherWiseThrowException(Token(RPARENS));

	if (copy.head().kind == ARROW) {
		Token arrow = copy.head();
		this->hasSideEffects = false;//!arrow.is_functional;
		copy.consume(1);
	} else {
		throw "expected an arrow token here";
	}

	this->returnType = new TypeNode(copy);

	tokens.set(copy);
}

