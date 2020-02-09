#include <vector>
#include <iostream>

#include "Method.hpp"
#include "typenodes/Type.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/Token.hpp"
#include "Identifier.hpp"
#include "statements/Stmt.hpp"
#include "DeclaredArg.hpp"

using namespace std;

Method::Method(TokenList tokens, bool debug) {

	if (debug) {
		cout << "Method(...)" << endl;
		cout << "from: " << endl << tokens.toSourceCodeFragment() << "" << endl;
	}

	TokenList copy = tokens.copy();

	copy.expect(Token(FN));

	this->methodName = Identifier(&copy,debug).identifier;

	copy.expect(Token(LPARENS));

	//while there is no ')' up, continue parsing arguments
	Token next = copy.get(0);
	while (!(next.kind == RPARENS)) {
		if (this->arguments.size() > 0) {
			copy.expect(Token(COMMA));
		}
		this->arguments.push_back(new DeclaredArg(&copy, debug));
		next = copy.get(0);
	}

	copy.expect(Token(RPARENS));
	
	copy.expect(Token(ARROW));
	
	this->returnType = new Type(copy);

	copy.expect(Token(LCURLY));

	Token statement_next = copy.get(0);
	while (!(statement_next.kind != RCURLY)) {
		this->statements.push_back(new Stmt(copy,debug));
		statement_next = copy.get(0);
	}

	copy.expect(Token(RCURLY));

	tokens.set(copy);
}

