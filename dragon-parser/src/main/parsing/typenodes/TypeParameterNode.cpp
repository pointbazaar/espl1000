
//standard headers
#include <vector>
#include <set>

//project headers
#include "TypeParameterNode.hpp"
//	  compiler/parsing/astnodes/typenodes/TypeNode.cpp
#include "../../lexing/TokenList.hpp"
#include "basic_and_wrapped/IBasicAndWrappedTypeNode.hpp"

using namespace std;

TypeParameterNode::TypeParameterNode(TokenList tokens){
	IToken token = tokens.get(0);
	if (token instanceof TypeParameterIdentifierToken) {
		this.typeParameterIndex = ((TypeParameterIdentifierToken) token).type_parameter_number;
		tokens.consume(1);
	} else {
		throw "could not read type parameter node";
	}
}

