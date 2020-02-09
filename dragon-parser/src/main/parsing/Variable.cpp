#include <iostream>
#include <vector>
#include <optional>

#include "Variable.hpp"
#include "SimpleVar.hpp"
#include "../commandline/Token.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/TokenList.hpp"

Variable::Variable(TokenList tokens, bool debug) {

	if(debug){
		cout << "Variable(...)" << endl;
	}

	TokenList copy = tokens.copy();

	this->simpleVariableNode = new SimpleVar(copy,debug);

	if (copy.size() > 0) {
		Token next = copy.get(0);
		while (next.kind == STRUCTMEMBERACCESS) {

			copy.expectAndConsumeOtherWiseThrowException(Token(STRUCTMEMBERACCESS));
			this->memberAccessList.push_back(new Variable(copy,debug));
			if (copy.size() > 0) {
				next = copy.get(0);
			} else {
				break;
			}
		}
	}

	tokens.set(copy);
}

