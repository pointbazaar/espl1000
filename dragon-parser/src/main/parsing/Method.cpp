#include <vector>
#include <iostream>

#include "Method.hpp"
#include "typenodes/Type.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/Token.hpp"
#include "Identifier.hpp"
#include "statements/Stmt.hpp"
#include "DeclArg.hpp"

using namespace std;

Method::Method(TokenList tokens, bool debug) {

	if (debug) {
		cout << "Method(...)" << endl;
		cout << "from: " << tokens.code() << "" << endl;
	}

	TokenList copy = tokens.copy();

	copy.expect(Token(FN));

	this->methodName = Identifier(&copy,debug).identifier;

	cout << "1" << endl;

	copy.expect(Token(LPARENS));

	cout << "2" << endl;

	//while there is no ')' up, continue parsing arguments
	Token next = copy.get(0);
	while (!(next.kind == RPARENS)) {
		if (this->arguments.size() > 0) {
			copy.expect(Token(COMMA));
		}
		this->arguments.push_back(new DeclArg(&copy, debug));
		next = copy.get(0);
	}

	cout << "4" << endl;

	copy.expect(Token(RPARENS));

	copy.expect(Token(ARROW));

	cout << "7" << endl;

	cout << copy.code() << endl;
	
	this->returnType = new Type(copy,debug);

	cout << "9" << endl;

	copy.expect(Token(LCURLY));

	cout << "11" << endl;

	Token statement_next = copy.get(0);
	while (!(statement_next.kind != RCURLY)) {
		this->statements.push_back(new Stmt(copy,debug));
		statement_next = copy.get(0);
	}

	cout << "13" << endl;

	copy.expect(Token(RCURLY));

	tokens.set(copy);
}

