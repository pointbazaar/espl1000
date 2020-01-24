
//standard headers
#include <vector>
#include <set>

//project headers
#include "StructMemberDeclNode.hpp"

class StructMemberDeclNode : IASTNode {

public:
	BasicTypeWrappedNode typeNode;
	string name;

	StructMemberDeclNode(TokenList tokens, bool debug) throws Exception {
		if (debug) {
			System.out.println("try to parse " + this.getClass().getSimpleName() + " from '" + tokens.toSourceCodeFragment() + "'");
		}

		final TokenList copy = tokens.copy();

		this.typeNode = new BasicTypeWrappedNode(copy);
		this.name = new IdentifierNode(copy).identifier;

		tokens.set(copy);
	}

};