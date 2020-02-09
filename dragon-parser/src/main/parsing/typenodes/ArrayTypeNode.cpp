
#include <vector>
#include "ArrayTypeNode.hpp"
#include "TypeNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"

ArrayTypeNode::ArrayTypeNode(TypeNode* element_type){
	this->element_type = element_type;
}

ArrayTypeNode::ArrayTypeNode(TokenList tokens) {

	TokenList copy1 = tokens.copy();

	copy1.expectAndConsumeOtherWiseThrowException(Token(LBRACKET));
	this->element_type = new TypeNode(copy1);
	copy1.expectAndConsumeOtherWiseThrowException(Token(RBRACKET));

	tokens.set(copy1);
}

