#include <iostream>

#include "RetStmt.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/Token.hpp"
#include "../Expr.hpp"

RetStmt::RetStmt(TokenList tokens, bool debug){

	if(debug){
		cout << "RetStmt(...)" << endl;
	}

	TokenList copy = TokenList(tokens);

	copy.expect(Token(RETURN));

	this->returnValue = new Expr(copy,debug);

	copy.expect(Token(SEMICOLON));

	tokens.set(copy);
}

