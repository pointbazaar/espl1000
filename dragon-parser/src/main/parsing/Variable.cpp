#include <iostream>
#include <vector>

#include "Variable.hpp"
#include "SimpleVar.hpp"
#include "../commandline/Token.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/TokenList.hpp"

struct Variable* makeVariable(TokenList* tokens, bool debug) {

	struct Variable* res = (struct Variable*)malloc(sizeof(struct Variable));
	res->simpleVariableNode = NULL;
	res->memberAccessList = vector<struct Variable*>();

	if(debug){
		cout << "Variable(...)" << endl;
		cout << "from " << tokens->code() << endl;
	}

	TokenList copy = tokens->copy();

	res->simpleVariableNode = new SimpleVar(&copy,debug);

	if (copy.size() >= 2) {
		Token next = copy.get(0);
		while (next.kind == STRUCTMEMBERACCESS) {

			copy.expect(Token(STRUCTMEMBERACCESS));
			res->memberAccessList.push_back(makeVariable(&copy,debug));
			if (copy.size() > 0) {
				next = copy.get(0);
			} else {
				break;
			}
		}
	}

	tokens->set(copy);

	return res;
}

