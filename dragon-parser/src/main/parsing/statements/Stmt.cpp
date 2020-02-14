
#include <vector>
#include <iostream>

#include "Stmt.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/Token.hpp"
#include "WhileStmt.hpp"
#include "IfStmt.hpp"
#include "RetStmt.hpp"
#include "MethodCall.hpp"
#include "AssignStmt.hpp"

Stmt::Stmt(TokenList tokens, bool debug) {

	if(debug){
		cout << "Stmt(...)" << endl;
	}

	TokenList copy = tokens.copy();

	if (copy.size() == 0) {
		throw "tried to parse a Statement, but there are no tokens left";
	}

	Token first = copy.get(0);

	if (first.kind == LOOP) {
		//this->statementNode = LoopStatementNode(copy);
	} else if (first.kind == WHILE) {
		this->m2 = new WhileStmt	(copy,debug);
	} else if (first.kind == IF) {
		this->m3 = makeIfStmt		(&copy,debug);
	} else if (first.kind == RETURN) {
		this->m4 = new RetStmt		(copy,debug);
	} else {
		//TODO: we have to figure something out here.
		//i don't want 'let' statements
		//because it just messes up the code and is
		//just bloat to write.
		//but parsing should be straightforward. to give good error messages

		try {
			TokenList copy2 = copy.copy();
			this->m1 = makeMethodCall(copy2,debug);
			copy2.expect(Token(SEMICOLON));
			copy.set(copy2);
		} catch (string e1) {
			this->m5 = new AssignStmt(copy,debug);
		}
	}

	tokens.set(copy);
}

