
//standard headers
#include <vector>
#include <set>

//project headers
#include "TypeNode.hpp"

class TypeNode : IASTNode {

public:
	ITypeNode typeNode;

	TypeNode(ITypeNode typeNode) {
		this.typeNode = typeNode;
	}

	TypeNode(TokenList tokens) throws Exception {

		TokenList copy = tokens.copy();

		try {
			this.typeNode = new ArrayTypeNode(copy);
		} catch (Exception e) {
			try {
				this.typeNode = new TypeParameterNode(copy);
			} catch (Exception e2) {
				this.typeNode = new BasicTypeWrappedNode(copy);
			}
		}

		tokens.set(copy);
	}

}
