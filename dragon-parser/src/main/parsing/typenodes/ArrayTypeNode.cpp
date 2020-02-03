
//standard headers
#include <vector>
#include <set>

//project headers
#include "ArrayTypeNode.hpp"
#include "ITypeNode.hpp"
#include "TypeNode.hpp"
#include "../../lexing/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"

using namespace std;

ArrayTypeNode::ArrayTypeNode(TypeNode element_type) {
	this->element_type = element_type;
}

ArrayTypeNode::ArrayTypeNode(TokenList tokens) {

	TokenList copy1 = tokens.copy();

	copy1.expectAndConsumeOtherWiseThrowException(BaseToken(LBRACKET));
	this->element_type = TypeNode(copy1);
	copy1.expectAndConsumeOtherWiseThrowException(BaseToken(RBRACKET));


	tokens.set(copy1);
}

