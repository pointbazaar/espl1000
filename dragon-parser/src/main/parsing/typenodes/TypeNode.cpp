
#include <vector>
#include <variant>
#include "TypeNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "ArrayTypeNode.hpp"
#include "TypeParameterNode.hpp"
#include "BasicTypeWrappedNode.hpp"


TypeNode::TypeNode(BasicTypeWrappedNode* typeNode){
	this->m1 = typeNode;
}

TypeNode::TypeNode(ArrayTypeNode* typeNode){
	this->m3 = typeNode;
}

TypeNode::TypeNode(TypeParameterNode* typeNode){
	this->m2 = typeNode;
}

TypeNode::TypeNode(TokenList tokens){

	TokenList copy = tokens.copy();

	try {
		this->m3 = new ArrayTypeNode(copy);
	} catch (std::string e) {
		try {
			this->m2 = new TypeParameterNode(copy);
		} catch (string e2) {
			this->m1 = new BasicTypeWrappedNode(copy);
		}
	}


	tokens.set(copy);
}

