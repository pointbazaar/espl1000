#include <iostream>
#include <vector>
#include <optional>

//project headers
#include "VariableNode.hpp"
#include "SimpleVariableNode.hpp"
#include "../commandline/Token.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/TokenList.hpp"

VariableNode::VariableNode(TokenList tokens, bool debug) {

	if(debug){
		cout << "VariableNode(...)" << endl;
	}

	TokenList copy = tokens.copy();

	this->simpleVariableNode = new SimpleVariableNode(copy,debug);

	if (copy.size() > 0) {
		Token next = copy.get(0);
		while (next.kind == STRUCTMEMBERACCESS) {

			copy.expectAndConsumeOtherWiseThrowException(Token(STRUCTMEMBERACCESS));
			this->memberAccessList.push_back(new VariableNode(copy,debug));
			if (copy.size() > 0) {
				next = copy.get(0);
			} else {
				break;
			}
		}
	}

	tokens.set(copy);
}

