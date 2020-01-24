
//standard headers
#include <vector>
#include <set>

//project headers
#include "TypeParameterNode.hpp"
//	  compiler/parsing/astnodes/typenodes/TypeNode.cpp
#include "../../../lexing/TokenList.hpp"

class TypeParameterNode : IBasicAndWrappedTypeNode, ITypeNode {

public:
	int typeParameterIndex;

	TypeParameterNode(TokenList tokens) throws Exception {
		IToken token = tokens.get(0);
		if (token instanceof TypeParameterIdentifierToken) {
			this.typeParameterIndex = ((TypeParameterIdentifierToken) token).type_parameter_number;
			tokens.consume(1);
		} else {
			throw new Exception("could not read type parameter node");
		}
	}

};