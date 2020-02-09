
#include <vector>
#include <optional>
#include <iostream>

#include "AssignStmt.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../typenodes/Type.hpp"
#include "../Variable.hpp"
#include "../Expr.hpp"

AssignStmt::AssignStmt(TokenList tokens,bool debug) {

	if(debug){
		cout << "AssignStmt(...)" << endl;
	}

	optional<Type*> optTypeNode1; 

	TokenList copy = tokens.copy();

	try {
		TokenList copy2 = copy.copy();
		optTypeNode1 = optional<Type*>(new Type(copy2));
		copy.set(copy2);
	} catch (string e) {
		optTypeNode1 = nullopt;
		//pass
	}

	this->optTypeNode = optTypeNode1;
	this->variableNode = new Variable(copy,debug);

	copy.expect(Token(OPKEY,"="));

	this->expressionNode = new Expr(copy,debug);

	copy.expect(Token(SEMICOLON));

	tokens.set(copy);
}

