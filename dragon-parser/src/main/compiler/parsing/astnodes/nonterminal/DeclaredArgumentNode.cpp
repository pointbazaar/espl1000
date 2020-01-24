
//standard headers
#include <vector>
#include <set>
#include <optional>

//project headers
#include "DeclaredArgumentNode.hpp"

class DeclaredArgumentNode : IASTNode {

public:
	TypeNode type;
	optional<String> name;

	DeclaredArgumentNode(TokenList tokens) throws Exception {

		TokenList copy = tokens.copy();

		this.type = new TypeNode(copy);
		this.name = Optional.of(new IdentifierNode(copy).identifier);

		tokens.set(copy);
	}

};