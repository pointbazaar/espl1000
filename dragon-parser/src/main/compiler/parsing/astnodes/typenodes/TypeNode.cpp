
//standard headers
#include <vector>
#include <set>

//project headers
#include "TypeNode.hpp"

public final class TypeNode implements IASTNode {

	public ITypeNode typeNode;

	public TypeNode(ITypeNode typeNode) {
		this.typeNode = typeNode;
	}

	public TypeNode(TokenList tokens) throws Exception {

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
