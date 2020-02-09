#include <iostream>
#include <vector>
#include <string>
#include "OperatorNode.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/BaseToken.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;


OperatorNode::OperatorNode(TokenList tokens, bool debug) {

	if(debug){
		cout << "OperatorNode(...)" << endl;
	}

	TokenList copy = TokenList(tokens);

	BaseToken token = copy.get(0);

	if (token.kind == OPERATOR) {

		if (copy.get(1).kind == OPERATOR) {
			BaseToken opleft = token;
			BaseToken opright = copy.get(1);

			if ((opleft.value.compare("<")==0) || opleft.value.compare(">")==0
					&& opright.value.compare("=")==0
			) {
				this->op = opleft.value + opright.value;
				copy.consume(2);
			} else if (opleft.value.compare("=")==0 && opright.value.compare("=")==0) {
				this->op = "==";
				copy.consume(2);
			} else {
				throw "could not recognize operator token: got " + token.value;
			}
		} else {
			this->op = (token).value;
			copy.consume(1);
		}
	} else {
		throw "could not recognize operator token: got " + token.value;
	}

	tokens.set(copy);
}

