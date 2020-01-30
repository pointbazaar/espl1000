
//standard headers
#include <vector>
#include <set>

//project headers
#include "TypeParameterNode.hpp"
#include "../../lexing/TokenList.hpp"
#include "IBasicAndWrappedTypeNode.hpp"

using namespace std;

TypeParameterNode::TypeParameterNode(TokenList tokens){
	IToken token = tokens.get(0);
	if (token->kind == TypeParameterIdentifierToken) {
		this->typeParameterIndex = ((TypeParameterIdentifierToken) token).type_parameter_number;
		tokens.consume(1);
	} else {
		throw "could not read type parameter node";
	}
}

