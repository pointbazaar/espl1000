#include <iostream>
#include <vector>
#include <string>
#include <sstream>

//project headers
#include "SimpleVar.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/Token.hpp"
#include "../commandline/TokenList.hpp"
#include "Expr.hpp"

using namespace std;

struct SimpleVar* makeSimpleVar(TokenList* tokens, bool debug) {

	if(debug){
		cout << "SimpleVar(...)" << endl;
		cout << "from " << tokens->code() << endl;
	}

	struct SimpleVar* res = (struct SimpleVar*)malloc(sizeof(struct SimpleVar));

	res->indexOptional = NULL;

	TokenList copy = TokenList(*tokens);

	if(copy.size()==0){
		throw string("no tokens");
	}

	Token token = copy.get(0);

	if (token.kind == ID) {
		res->name = (char*)token.value.c_str();
		copy.consume(1);

		//it could have an index
		if (copy.size() > 0 && copy.get(0).kind == LBRACKET) {
			copy.expect(LBRACKET,"[");
			res->indexOptional = makeExpr(&copy,debug);
			copy.expect(RBRACKET,"]");
		} else {
			res->indexOptional = NULL;
			//pass, this assignment has no index to it
		}

	} else {
		stringstream msg;
		msg << tokens->relPath 
		<< string(":") 
		<< token.lineNumber
		<< ": could not read variable name. token was " 
		<< token.value
		<< " from context  '" 
		<< tokens->code()
		<< "'";

		throw msg.str();
	}

	tokens->set(copy);

	return res;
}

