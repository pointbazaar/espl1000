#include <vector>
#include <iostream>

#include "MethodCall.hpp"
#include "../Identifier.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../Expr.hpp"

MethodCall::MethodCall(TokenList tokens,bool debug) {

	if(debug){
		cout << "MethodCallNode(...)" << endl;
	}

	TokenList copy = tokens.copy();

	this->methodName = Identifier(&copy,debug).identifier;

	copy.expectAndConsumeOtherWiseThrowException(Token(LPARENS));

	//while there is no ')' up, continue parsing arguments
	Token next = copy.get(0);
	while (!(next.kind == RPARENS)) {
		if (arguments.size() > 0) {
			copy.expectAndConsumeOtherWiseThrowException(Token(COMMA));
		}
		this->arguments.push_back(new Expr(copy,debug));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(Token(RPARENS));

	tokens.set(copy);
}

