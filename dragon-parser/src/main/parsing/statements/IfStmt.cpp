
#include <vector>
#include <string>
#include <map>
#include <stdio.h>
#include <iostream>

#include "IfStmt.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "../../commandline/TokenList.hpp"
#include "../Expr.hpp"
#include "Stmt.hpp"

using namespace std;

struct IfStmt* makeIfStmt(TokenList* tokens, bool debug) {

	if(debug){
		cout << "IfStmt(...)" << endl; 
	}

	struct IfStmt* res = (struct IfStmt*)malloc(sizeof(struct IfStmt));
	
	res->condition = NULL;
	
	res->count_statements = 0;
	res->statements 	= (struct Stmt**)malloc(sizeof(struct Stmt*)*1);

	res->count_elseStatements = 0;
	res->elseStatements = (struct Stmt**)malloc(sizeof(struct Stmt*)*1);

	TokenList copy = TokenList(*tokens);

	if(copy.size() < 3){
		throw string("not enough tokens");
	}

	copy.expect(IF);
	res->condition = makeExpr(&copy,debug);
	copy.expect(LCURLY);

	Token next = copy.get(0);
	
	while (next.kind != RCURLY) {

		res->statements[res->count_statements] = makeStmt(&copy,debug);
		res->count_statements++;
		int newsize = sizeof(struct Stmt*) * res->count_statements+1;
		res->statements = (struct Stmt**)realloc(res->statements,newsize);

		next = copy.get(0);
	}

	copy.expect(RCURLY);

	//maybe there is an else
	if (copy.startsWith(ELSE)) {

		copy.expect(ELSE);

		copy.expect(LCURLY);

		//maybe there be some statements
		Token elsenext = copy.get(0);

		while (elsenext.kind != RCURLY) {

			res->elseStatements[res->count_elseStatements] = makeStmt(&copy,debug);
			res->count_elseStatements++;
			int newsize2 = sizeof(struct Stmt*) * res->count_elseStatements+1;
			res->elseStatements = (struct Stmt**)realloc(res->elseStatements,newsize2);

			elsenext = copy.get(0);
		}

		copy.expect(RCURLY);
	}

	tokens->set(copy);

	return res;
}

