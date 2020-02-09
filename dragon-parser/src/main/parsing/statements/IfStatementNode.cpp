
//standard headers
#include <vector>
#include <string>
#include <map>
#include <stdio.h>

//project headers
#include "IfStatementNode.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/TokenList.hpp"
#include "../ExpressionNode.hpp"
#include "StatementNode.hpp"

using namespace std;

IfStatementNode::IfStatementNode(TokenList tokens, bool debug) {

	if(debug){
		cout << "IfStatementNode(...)" << endl; 
	}

	TokenList copy = TokenList(tokens);
	copy.expectAndConsumeOtherWiseThrowException(Token(IF));
	this->condition = new ExpressionNode(copy,debug);
	copy.expectAndConsumeOtherWiseThrowException(Token(LCURLY));

	Token next = copy.get(0);

	while (!(next.kind == RCURLY)) {

		this->statements.push_back(new StatementNode(copy,debug));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(Token(RCURLY));

	//maybe there is an else
	if (copy.startsWith(Token(ELSE))) {

		copy.expectAndConsumeOtherWiseThrowException(Token(ELSE));

		copy.expectAndConsumeOtherWiseThrowException(Token(LCURLY));

		//maybe there be some statements
		Token elsenext = copy.get(0);

		while (!(elsenext.kind == RCURLY)) {

			this->elseStatements.push_back(new StatementNode(copy,debug));
			elsenext = copy.get(0);
		}

		copy.expectAndConsumeOtherWiseThrowException(Token(RCURLY));
	}

	tokens.set(copy);
}

