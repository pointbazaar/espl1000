
//standard headers
#include <vector>
#include <optional>
#include <iostream>
//project headers
#include "AssignmentStatementNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../typenodes/TypeNode.hpp"
#include "../VariableNode.hpp"
#include "../ExpressionNode.hpp"

AssignmentStatementNode::AssignmentStatementNode(TokenList tokens,bool debug) {

	if(debug){
		cout << "AssignmentStatementNode(...)" << endl;
	}

	optional<TypeNode*> optTypeNode1; 

	TokenList copy = tokens.copy();

	try {
		TokenList copy2 = copy.copy();
		optTypeNode1 = optional<TypeNode*>(new TypeNode(copy2));
		copy.set(copy2);
	} catch (string e) {
		optTypeNode1 = nullopt;
		//pass
	}

	this->optTypeNode = optTypeNode1;
	this->variableNode = new VariableNode(copy,debug);

	copy.expectAndConsumeOtherWiseThrowException(Token(OPERATOR,"="));

	this->expressionNode = new ExpressionNode(copy,debug);

	copy.expectAndConsumeOtherWiseThrowException(Token(SEMICOLON));

	tokens.set(copy);
}

