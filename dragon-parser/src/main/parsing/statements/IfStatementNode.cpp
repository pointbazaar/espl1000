
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
	copy.expectAndConsumeOtherWiseThrowException(BaseToken(IF));
	this->condition = new ExpressionNode(copy,debug);
	copy.expectAndConsumeOtherWiseThrowException(BaseToken(LCURLY));

	BaseToken next = copy.get(0);

	while (!(next.kind == RCURLY)) {

		this->statements.push_back(new StatementNode(copy,debug));
		next = copy.get(0);
	}

	copy.expectAndConsumeOtherWiseThrowException(BaseToken(RCURLY));

	//maybe there is an else
	if (copy.startsWith(BaseToken(ELSE))) {

		copy.expectAndConsumeOtherWiseThrowException(BaseToken(ELSE));

		copy.expectAndConsumeOtherWiseThrowException(BaseToken(LCURLY));

		//maybe there be some statements
		BaseToken elsenext = copy.get(0);

		while (!(elsenext.kind == RCURLY)) {

			this->elseStatements.push_back(new StatementNode(copy,debug));
			elsenext = copy.get(0);
		}

		copy.expectAndConsumeOtherWiseThrowException(BaseToken(RCURLY));
	}

	tokens.set(copy);
}

