
//standard headers
#include <vector>
#include <optional>
#include <string>

//project headers
#include "DeclaredArgumentNode.hpp"
#include "../commandline/TokenList.hpp"
#include "typenodes/TypeNode.hpp"
#include "IdentifierNode.hpp"

using namespace std;

DeclaredArgumentNode::DeclaredArgumentNode(TokenList tokens) {

	TokenList copy = tokens.copy();

	this->type = new TypeNode(copy);

	this->name = optional<string>(IdentifierNode(copy).identifier);

	tokens.set(copy);
}
