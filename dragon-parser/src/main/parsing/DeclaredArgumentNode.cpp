
//standard headers
#include <vector>
#include <set>
#include <optional>
#include <string>

//project headers
#include "DeclaredArgumentNode.hpp"
#include "TokenList.hpp"
#include "TypeNode.hpp"

using namespace std;

DeclaredArgumentNode::DeclaredArgumentNode(TokenList tokens) {

	TokenList copy = tokens.copy();

	this.type = new TypeNode(copy);
	this.name = Optional.of(new IdentifierNode(copy).identifier);

	tokens.set(copy);
}

