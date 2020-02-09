
#include <vector>
#include <optional>
#include <string>
#include <iostream>
//project headers
#include "DeclaredArgumentNode.hpp"
#include "../commandline/TokenList.hpp"
#include "typenodes/TypeNode.hpp"
#include "IdentifierNode.hpp"

using namespace std;

DeclaredArgumentNode::DeclaredArgumentNode(TokenList tokens, bool debug) {

	if(debug){
		cout << "DeclaredArgumentNode(...)" << endl;
	}

	TokenList copy = tokens.copy();

	this->type = new TypeNode(copy);

	this->name = optional<string>(IdentifierNode(&copy,debug).identifier);

	tokens.set(copy);
}

