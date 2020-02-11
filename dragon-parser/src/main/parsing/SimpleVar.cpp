#include <iostream>
#include <vector>
#include <set>
#include <optional>
#include <string>
#include <sstream>

//project headers
#include "SimpleVar.hpp"
#include "../commandline/TokenKeys.hpp"
#include "Expr.hpp"

using namespace std;

SimpleVar::SimpleVar(TokenList tokens, bool debug) {

	if(debug){
		cout << "SimpleVar(...)" << endl;
		cout << "from " << tokens.code() << endl;
	}

	TokenList copy = TokenList(tokens);

	if(copy.size()==0){
		throw string("no tokens");
	}

	Token token = copy.get(0);

	if (token.kind == ID) {
		this->name = token.value;
		copy.consume(1);

		//it could have an index
		if (copy.size() > 0 && copy.get(0).kind == LBRACKET) {
			copy.expect(Token(LBRACKET,"["));
			this->indexOptional = optional(makeExpr(&copy,debug));
			copy.expect(Token(RBRACKET,"]"));
		} else {
			this->indexOptional = nullopt;
			//pass, this assignment has no index to it
		}

	} else {
		stringstream msg;
		msg << tokens.relPath 
		<< string(":") 
		<< token.lineNumber
		<< ": could not read variable name. token was " 
		<< token.value
		<< " from context  '" 
		<< tokens.code()
		<< "'";

		throw msg.str();
	}

	tokens.set(copy);
}

