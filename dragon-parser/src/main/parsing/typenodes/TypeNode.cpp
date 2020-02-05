
//standard headers
#include <vector>
#include <set>

//project headers
#include "TypeNode.hpp"
#include "ITypeNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "ArrayTypeNode.hpp"
#include "TypeParameterNode.hpp"
#include "BasicTypeWrappedNode.hpp"

using namespace std;

TypeNode::TypeNode(ITypeNode typeNode) {
	this->typeNode = typeNode;
}

TypeNode::TypeNode(TokenList tokens){

	TokenList copy = tokens.copy();

	try {
		this->typeNode = ArrayTypeNode(copy);
	} catch (string e) {
		try {
			this->typeNode = TypeParameterNode(copy);
		} catch (string e2) {
			this->typeNode = BasicTypeWrappedNode(copy);
		}
	}

	tokens.set(copy);
}

