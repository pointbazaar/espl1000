
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

struct Stmt* makeStmt(TokenList* tokens, bool debug) {

	if(debug){
		cout << "Stmt(...)" << endl;
	}

	struct Stmt* res = (struct Stmt*)malloc(sizeof(struct Stmt));

	//init
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;

	TokenList copy = tokens->copy();

	if (copy.size() == 0) {
		throw "tried to parse a Statement, but there are no tokens left";
	}

	Token first = copy.get(0);

	if (first.kind == LOOP) {
		//this->statementNode = LoopStatementNode(copy);
	} else if (first.kind == WHILE) {
		res->m2 = makeWhileStmt		(&copy,debug);
	} else if (first.kind == IF) {
		res->m3 = makeIfStmt		(&copy,debug);
	} else if (first.kind == RETURN) {
		res->m4 = makeRetStmt		(&copy,debug);
	} else {
		//TODO: we have to figure something out here.
		//i don't want 'let' statements
		//because it just messes up the code and is
		//just bloat to write.
		//but parsing should be straightforward. to give good error messages

		try {

			TokenList copy2 = copy.copy();
			res->m1 = makeMethodCall(&copy2,debug);
			copy2.expect(SEMICOLON);
			copy.set(copy2);

		} catch (string e1) {
			res->m5 = makeAssignStmt(&copy,debug);
		}
	}

	tokens->set(copy);

	return res;
}

