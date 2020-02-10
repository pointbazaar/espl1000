#include <vector>
#include <optional>
#include <string>
#include <iostream>

#include "DeclArg.hpp"
#include "../commandline/TokenList.hpp"
#include "typenodes/Type.hpp"
#include "Identifier.hpp"

using namespace std;

DeclArg::DeclArg(TokenList* tokens, bool debug) {

	if(debug){
		cout << "DeclaredArg(...)" << endl;
		cout << "from " << tokens->code() << endl;
	}

	TokenList copy = tokens->copy();

	this->type = new Type(copy,debug);

	this->name = optional<string>(Identifier(&copy,debug).identifier);

	tokens->set(copy);
}

