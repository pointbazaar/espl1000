
//standard headers
#include <vector>
#include <set>

//project headers
#include "StructMemberDeclNode.hpp"

public final class StructMemberDeclNode implements IASTNode {

	public final BasicTypeWrappedNode typeNode;
	public final String name;

	public StructMemberDeclNode(final TokenList tokens, boolean debug) throws Exception {
		if (debug) {
			System.out.println("try to parse " + this.getClass().getSimpleName() + " from '" + tokens.toSourceCodeFragment() + "'");
		}

		final TokenList copy = tokens.copy();

		this.typeNode = new BasicTypeWrappedNode(copy);
		this.name = new IdentifierNode(copy).identifier;

		tokens.set(copy);
	}

}
