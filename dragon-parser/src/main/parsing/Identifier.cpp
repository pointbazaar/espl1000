#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "Identifier.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/Token.hpp"
#include "../commandline/TokenKeys.hpp"

#include <stdio.h>

using namespace std;

struct Identifier* makeIdentifier(TokenList* tokens, bool debug) {

	if(debug){
		cout << "Identifier(...)" << endl;
		cout << "from: " << tokens->code() << endl;
	}

	struct Identifier* res = (struct Identifier*)malloc(sizeof(struct Identifier));

	if(res==NULL){
		printf("could not malloc.");
		exit(1);
	}

	Token token = tokens->get(0);

	if (token.kind == ID) {
		res->identifier = (char*)token.value.c_str();
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

	return res;
}

