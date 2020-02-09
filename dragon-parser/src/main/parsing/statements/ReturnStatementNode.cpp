#include <iostream>
#include "ReturnStatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/BaseToken.hpp"
#include "../ExpressionNode.hpp"

ReturnStatementNode::ReturnStatementNode(TokenList tokens, bool debug){

	if(debug){
		cout << "ReturnStatementNode(...)" << endl;
	}

	TokenList copy = TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RETURN));

	this->returnValue = new ExpressionNode(copy,debug);

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(SEMICOLON));

	tokens.set(copy);
}

