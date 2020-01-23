
//standard headers
#include <vector>
#include <set>

//project headers
#include "TypeParameterNode.hpp"

public final class TypeParameterNode implements IBasicAndWrappedTypeNode, ITypeNode {

	public final int typeParameterIndex;

	public TypeParameterNode(final TokenList tokens) throws Exception {
		IToken token = tokens.get(0);
		if (token instanceof TypeParameterIdentifierToken) {
			this.typeParameterIndex = ((TypeParameterIdentifierToken) token).type_parameter_number;
			tokens.consume(1);
		} else {
			throw new Exception("could not read type parameter node");
		}
	}

}
