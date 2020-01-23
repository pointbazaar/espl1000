
//standard headers
#include <vector>
#include <set>

//project headers
#include "DeclaredArgumentNode.hpp"

public final class DeclaredArgumentNode implements IASTNode {

	public final TypeNode type;

	public final Optional<String> name;

	public DeclaredArgumentNode(final TokenList tokens) throws Exception {

		final TokenList copy = tokens.copy();

		this.type = new TypeNode(copy);
		this.name = Optional.of(new IdentifierNode(copy).identifier);

		tokens.set(copy);
	}

}
