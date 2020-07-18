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
#include "../../../../util/util.h"

struct Method* makeMethod(struct TokenList* tokens, bool debug) {

	if (debug) {
		printf("Method(...) from: ");
		list_print(tokens);
	}

	struct Method* res = smalloc(sizeof(struct Method));

	//init
	res->isPublic = true;
	res->hasSideEffects = true;
	res->count_args = 0;
	res->args = smalloc(sizeof(struct DeclArg*)*1);
	res->count_stmts = 0;
	res->stmts = smalloc(sizeof(struct Stmt*)*1);

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, FN)){
		//as a subroutine is parsed deterministically (we know to parse a subroutine by the 'fn' keyword),
		//give a little parse error message
		printf("expected 'fn', but was: %s\n", list_code(copy,debug));
		free(res);
		return NULL;
	}

	struct Identifier* id = makeIdentifier(copy,debug);
	if(id == NULL){
		printf("expected method name, but was: %s\n", list_code(copy, debug));
		free(res);
		return NULL;
	}

	strcpy(res->name, id->identifier);
	freeIdentifier(id);

	if(!list_expect(copy, LPARENS)){
		free(res);
		return NULL;
	}

	//while there is no ')' up, continue parsing arguments
	struct Token* next = list_head(copy);
	if(next == NULL){
		free(res);
		return NULL;
	}

	while (next->kind != RPARENS) {
		if (res->count_args > 0) {
			if(!list_expect(copy, COMMA)){return NULL;}
		}
		
		struct DeclArg* da = makeDeclArg(copy, debug);
		if(da == NULL){
			free(res);
			return NULL;
		}
		res->args[res->count_args] = da;
		res->count_args++;
		
		res->args = realloc(res->args,sizeof(struct DeclArg*)*(res->count_args+1));

		next = list_head(copy);
		if(next == NULL){
			free(res);
			return NULL;
		}
	}

	if(!list_expect(copy, RPARENS)){
		free(res);
		return NULL;
	}

	if(!list_expect(copy, ARROW)){
		free(res);
		return NULL;
	}
	
	res->returnType = makeType2(copy,debug);
	if(res->returnType == NULL){
		free(res);
		return NULL;
	}

	if(!list_expect(copy, LCURLY)){
		free(res);
		return NULL;
	}

	struct Token* tk_next = list_head(copy);
	if(tk_next == NULL){
		free(res);
		return NULL;
	}

	while (tk_next->kind != RCURLY) {

		struct Stmt* mystmt = makeStmt(copy, debug);
		if(mystmt == NULL){
			free(res);
			return NULL;
		}

		res->stmts[res->count_stmts] = mystmt;
		res->count_stmts += 1;
		
		size_t newsize = sizeof(struct Stmt*)*(res->count_stmts+1);
		res->stmts = realloc(res->stmts, newsize);

		tk_next = list_head(copy);
		if(tk_next == NULL){
			free(res);
			return NULL;
		}
	}

	if(!list_expect(copy, RCURLY)){
		free(res);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing Method\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

void freeMethod(struct Method* m){
	
	printf("DEBUG: freeMethod\n");
	
	freeType(m->returnType);
	for(int i=0;i < m->count_args; i++){
		freeDeclArg(m->args[i]);
	}
	for(int i=0;i < m->count_stmts; i++){
		freeStmt(m->stmts[i]);
	}
	free(m);
}

