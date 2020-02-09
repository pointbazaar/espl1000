#include <iostream>
#include "ReturnStatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/Token.hpp"
#include "../ExpressionNode.hpp"

ReturnStatementNode::ReturnStatementNode(TokenList tokens, bool debug){

	if(debug){
		cout << "ReturnStatementNode(...)" << endl;
	}

	TokenList copy = TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(Token(RETURN));

	this->returnValue = new ExpressionNode(copy,debug);

	copy.expectAndConsumeOtherWiseThrowException(Token(SEMICOLON));

	tokens.set(copy);
}

