
//standard headers
#include <vector>
#include <iostream>
//project headers
#include "WhileStatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/Token.hpp"
#include "../ExpressionNode.hpp"
#include "StatementNode.hpp"

using namespace std;

WhileStatementNode::WhileStatementNode(TokenList tokens, bool debug){

	if(debug){
		cout << "WhileStatementNode(...)" << endl;
	}

	TokenList copy = TokenList(tokens);

	copy.expectAndConsumeOtherWiseThrowException(Token(WHILE));

	this->condition = new ExpressionNode(copy,debug);

	copy.expectAndConsumeOtherWiseThrowException(Token(LCURLY));

	Token next = copy.get(0);
	while (!(next.kind == RCURLY)) {
		this->statements.push_back(new StatementNode(copy,debug));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(Token(RCURLY));

	tokens.set(copy);
}

