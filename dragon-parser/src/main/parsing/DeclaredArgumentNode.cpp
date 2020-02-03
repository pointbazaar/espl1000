
//standard headers
#include <vector>
#include <optional>
#include <string>

//project headers
#include "DeclaredArgumentNode.hpp"
#include "../lexing/TokenList.hpp"
#include "typenodes/TypeNode.hpp"

using namespace std;

DeclaredArgumentNode::DeclaredArgumentNode(TokenList tokens) {

	TokenList copy = tokens.copy();

	this->type = TypeNode(copy);
	this->name = Optional.of(IdentifierNode(copy).identifier);

	tokens.set(copy);
}

