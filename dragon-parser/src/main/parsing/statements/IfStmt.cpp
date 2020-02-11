
#include <vector>
#include <string>
#include <map>
#include <stdio.h>

#include "IfStmt.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/TokenList.hpp"
#include "../Expr.hpp"
#include "Stmt.hpp"

using namespace std;

IfStmt::IfStmt(TokenList tokens, bool debug) {

	if(debug){
		cout << "IfStmt(...)" << endl; 
	}

	TokenList copy = TokenList(tokens);
	copy.expect(Token(IF));
	this->condition = makeExpr(&copy,debug);
	copy.expect(Token(LCURLY));

	Token next = copy.get(0);

	while (!(next.kind == RCURLY)) {

		this->statements.push_back(new Stmt(copy,debug));
		next = copy.get(0);
	}

	copy.expect(Token(RCURLY));

	//maybe there is an else
	if (copy.startsWith(Token(ELSE))) {

		copy.expect(Token(ELSE));

		copy.expect(Token(LCURLY));

		//maybe there be some statements
		Token elsenext = copy.get(0);

		while (!(elsenext.kind == RCURLY)) {

			this->elseStatements.push_back(new Stmt(copy,debug));
			elsenext = copy.get(0);
		}

		copy.expect(Token(RCURLY));
	}

	tokens.set(copy);
}

