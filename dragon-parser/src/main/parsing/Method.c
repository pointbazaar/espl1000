#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Method.h"
#include "typenodes/Type.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"
#include "../commandline/smalloc.h"
#include "Identifier.h"
#include "statements/Stmt.h"
#include "DeclArg.h"

struct Method* makeMethod(struct TokenList* tokens, bool debug) {

	if (debug) {
		printf("Method(...) from: %s\n", list_code(tokens));
	}

	struct Method* res = smalloc(sizeof(struct Method));

	//init
	res->isPublic = true;
	res->hasSideEffects = true;
	res->count_arguments = 0;
	res->arguments = smalloc(sizeof(struct DeclArg*)*1);
	res->count_statements = 0;
	res->statements = smalloc(sizeof(struct Stmt*)*1);

	struct TokenList* copy = list_copy(tokens);

	list_expect(copy, FN);

	res->methodName = makeIdentifier(copy,debug)->identifier;
	if(res->methodName == NULL){return NULL;}

	if(!list_expect(copy, LPARENS)){return NULL;}

	//while there is no ')' up, continue parsing arguments
	struct Token* next = list_get(copy, 0);
	if(next == NULL){return NULL;}

	while (next->kind != RPARENS) {
		if (res->count_arguments > 0) {
			if(!list_expect(copy, COMMA)){return NULL;}
		}
		res->arguments[res->count_arguments++] = makeDeclArg(copy, debug);
		res->arguments = realloc(res->statements,sizeof(struct DeclArg*)*(res->count_arguments+1));

		next = list_get(copy, 0);
		if(next == NULL){return NULL;}
	}

	if(!list_expect(copy, RPARENS)){return NULL;}

	if(!list_expect(copy, ARROW)){return NULL;}
	
	res->returnType = makeType2(copy,debug);
	if(res->returnType == NULL){return NULL;}

	if(!list_expect(copy, LCURLY)){return NULL;}

	struct Token* statement_next = list_get(copy, 0);
	if(statement_next == NULL){return NULL;}

	while (statement_next->kind != RCURLY) {

		struct Stmt* mystmt = makeStmt(copy, debug);
		if(mystmt == NULL){return NULL;}

		res->statements[res->count_statements++] = mystmt;
		res->statements = realloc(res->statements,sizeof(struct Stmt*)*(res->count_statements+1));

		statement_next = list_get(copy, 0);
		if(statement_next == NULL){return NULL;}
	}

	if(!list_expect(copy, RCURLY)){return NULL;}

	list_set(tokens, copy);

	return res;
}

