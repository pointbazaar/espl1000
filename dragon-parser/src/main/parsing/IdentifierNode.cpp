#include <iostream>
#include <vector>
#include <string>
#include <sstream>

//project headers
#include "IdentifierNode.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/BaseToken.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

IdentifierNode::IdentifierNode(TokenList* tokens, bool debug) {

	if(debug){
		cout << "IdentifierNode(...)" << endl;
	}

	BaseToken token = tokens->get(0);

	if (token.kind == IDENTIFIER) {
		this->identifier = token.value;
		tokens->consume(1);

	} else {
		stringstream msg;
		msg << "could not read identifier from token: " 
		<< token.value
		<< " in " << tokens->relPath << ":" 
		<< token.lineNumber
		<< " in context '" 
		<< tokens->toSourceCodeFragment()
		<< "'";

		throw msg.str();
	}

}

