#include <vector>
#include <optional>
#include <string>
#include <iostream>

#include "DeclaredArg.hpp"
#include "../commandline/TokenList.hpp"
#include "typenodes/Type.hpp"
#include "Identifier.hpp"

using namespace std;

DeclaredArg::DeclaredArg(TokenList* tokens, bool debug) {

	if(debug){
		cout << "DeclaredArg(...)" << endl;
		cout << "from " << tokens->toSourceCodeFragment() << endl;
	}

	TokenList copy = tokens->copy();

	this->type = new Type(copy,debug);

	this->name = optional<string>(Identifier(&copy,debug).identifier);

	tokens->set(copy);
}

