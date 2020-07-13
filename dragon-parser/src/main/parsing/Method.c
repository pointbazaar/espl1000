#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Method.h"
#include "typenodes/Type.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"
#include "Identifier.h"
#include "statements/Stmt.h"
#include "DeclArg.h"

struct Method* makeMethod(struct TokenList* tokens, bool debug) {

	if (debug) {
		printf("Method(...) from: %s\n", list_code(tokens, debug));
	}

	struct Method* res = malloc(sizeof(struct Method));

	//init
	res->isPublic = true;
	res->hasSideEffects = true;
	res->count_args = 0;
	res->args = malloc(sizeof(struct DeclArg*)*1);
	res->count_stmts = 0;
	res->stmts = malloc(sizeof(struct Stmt*)*1);

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, FN)){
		//as a subroutine is parsed deterministically (we know to parse a subroutine by the 'fn' keyword),
		//give a little parse error message
		printf("expected 'fn', but was: %s\n", list_code(copy,debug));
		return NULL;
	}

	struct Identifier* id = makeIdentifier(copy,debug);
	if(id == NULL){
		printf("expected method name, but was: %s\n", list_code(copy, debug));
		return NULL;
	}

	res->name = id->identifier;

	if(!list_expect(copy, LPARENS)){return NULL;}

	//while there is no ')' up, continue parsing arguments
	struct Token* next = list_head(copy);
	if(next == NULL){return NULL;}

	while (next->kind != RPARENS) {
		if (res->count_args > 0) {
			if(!list_expect(copy, COMMA)){return NULL;}
		}
		res->args[res->count_args++] = makeDeclArg(copy, debug);
		res->args = realloc(res->stmts,sizeof(struct DeclArg*)*(res->count_args+1));

		next = list_head(copy);
		if(next == NULL){return NULL;}
	}

	if(!list_expect(copy, RPARENS)){return NULL;}

	if(!list_expect(copy, ARROW)){return NULL;}
	
	res->returnType = makeType2(copy,debug);
	if(res->returnType == NULL){return NULL;}

	if(!list_expect(copy, LCURLY)){return NULL;}

	struct Token* tk_next = list_head(copy);
	if(tk_next == NULL){return NULL;}

	while (tk_next->kind != RCURLY) {

		struct Stmt* mystmt = makeStmt(copy, debug);
		if(mystmt == NULL){return NULL;}

		res->stmts[res->count_stmts] = mystmt;
		res->count_stmts += 1;
		
		res->stmts = realloc(res->stmts,sizeof(struct Stmt*)*(res->count_stmts+1));

		tk_next = list_head(copy);
		if(tk_next == NULL){return NULL;}
	}

	if(!list_expect(copy, RCURLY)){return NULL;}
	
	if(debug){
		printf("sucess parsing Method\n");
	}

	list_set(tokens, copy);

	return res;
}

