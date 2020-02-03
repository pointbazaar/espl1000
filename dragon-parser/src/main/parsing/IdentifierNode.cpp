
//standard headers
#include <vector>
#include <set>
#include <string>
#include <sstream>

//project headers
#include "IdentifierNode.hpp"
#include "../lexing/TokenList.hpp"
#include "../lexing/BaseToken.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

IdentifierNode::IdentifierNode(TokenList tokens) {

	BaseToken token = tokens.get(0);

	if (token.kind == IDENTIFIER) {
		this->identifier = token.getContents();
		tokens.consume(1);

	} else {
		stringstream msg;
		msg << "could not read identifier from token: " 
		<< token.getContents()
		<< " in " << tokens.relPath << ":" 
		<< token.lineNumber
		<< " in context '" 
		<< tokens.toSourceCodeFragment().substr(0, min(20, (int)tokens.toSourceCodeFragment().size())) 
		<< "'";

		throw (to_string(msg));
	}

}

