#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "Identifier.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/Token.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

Identifier::Identifier(TokenList* tokens, bool debug) {

	if(debug){
		cout << "Identifier(...)" << endl;
		cout << "from: " << tokens->code() << endl;
	}

	Token token = tokens->get(0);

	if (token.kind == ID) {
		this->identifier = token.value;
		tokens->consume(1);

	} else {
		stringstream msg;
		msg << "could not read identifier from token: " 
		<< token.value
		<< " in " << tokens->relPath << ":" 
		<< token.lineNumber
		<< " in context '" 
		<< tokens->code()
		<< "'";

		cout << msg.str() << endl;
		throw msg.str();
	}

}

