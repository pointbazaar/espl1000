
//standard headers
#include <vector>
#include <set>

//project headers
#include "SimpleTypeNode.hpp"

public final class SimpleTypeNode implements IBasicAndWrappedTypeNode {

	public final String typeName;

	public SimpleTypeNode(final TokenList tokens) throws Exception {
		if (tokens.size() == 0) {
			throw new Exception();
		}

		IToken token = tokens.get(0);
		if (token instanceof TypeIdentifierToken) {
			this.typeName =
					token.getContents();

			tokens.consume(1);
		} else if (token instanceof AnyTypeToken) {
			tokens.consume(1);
			this.typeName = "#";
		} else {
			throw new Exception("could not read simple type identifier");
		}
	}

	public SimpleTypeNode(String typeName) {
		this.typeName = typeName;
	}


}
