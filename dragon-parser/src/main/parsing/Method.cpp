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

struct Method* makeMethod(TokenList* tokens, bool debug) {

	if (debug) {
		cout << "Method(...)" << "from: " << tokens->code() << "" << endl;
	}

	struct Method* res = (struct Method*)malloc(sizeof(struct Method));

	//init
	res->count_arguments = 0;
	res->arguments = (struct DeclArg**)malloc(sizeof(struct DeclArg*)*1);
	res->count_statements = 0;
	res->statements = (struct Stmt**)malloc(sizeof(struct Stmt*)*1);

	TokenList copy = tokens->copy();

	copy.expect(FN);

	res->methodName = makeIdentifier(&copy,debug)->identifier;

	copy.expect(LPARENS);

	//while there is no ')' up, continue parsing arguments
	Token next = copy.get(0);
	while (!(next.kind == RPARENS)) {
		if (res->count_arguments > 0) {
			copy.expect(COMMA);
		}
		res->arguments[res->count_arguments++] = makeDeclArg(&copy, debug);
		res->arguments = (struct DeclArg**)realloc(res->statements,sizeof(struct DeclArg*)*(res->count_arguments+1));

		next = copy.get(0);
	}

	copy.expect(RPARENS);

	copy.expect(ARROW);

	cout << copy.code() << endl;
	
	res->returnType = makeType(&copy,debug);

	copy.expect(LCURLY);

	Token statement_next = copy.get(0);
	while (!(statement_next.kind != RCURLY)) {

		res->statements[res->count_statements++] = makeStmt(&copy, debug);
		res->statements = (struct Stmt**)realloc(res->statements,sizeof(struct Stmt*)*(res->count_statements+1));

		statement_next = copy.get(0);
	}

	copy.expect(RCURLY);

	tokens->set(copy);

	return res;
}

