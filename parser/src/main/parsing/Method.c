#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Method.h"
#include "StmtBlock.h"
#include "typenodes/Type.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"
#include "Identifier.h"
#include "statements/Stmt.h"
#include "DeclArg.h"
#include "../../../../util/util.h"
#include "../../../../ast/free_ast.h"

// --- private subroutines ---
struct Method* initMethod();
bool tryParseArgList(struct Method* res, struct TokenList* copy, bool debug);
// ---------------------------

struct Method* makeMethod(struct TokenList* tokens, bool debug) {

	if (debug) {
		printf("Method(...) from: ");
		list_print(tokens);
	}

	struct Method* res = initMethod();

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

	if(!tryParseArgList(res, copy, debug)){
		free(res);
		return NULL;
	}

	if(!list_expect(copy, ARROW)){
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	res->returnType = makeType2(copy,debug);
	if(res->returnType == NULL){
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	res->block = makeStmtBlock(copy, debug);
	if(res->block == NULL){
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing Method\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

struct Method* initMethod(){
	
	struct Method* res = smalloc(sizeof(struct Method));

	res->isPublic = true;
	res->hasSideEffects = true;
	res->count_args = 0;
	res->args = smalloc(sizeof(struct DeclArg*)*1);
	res->block = NULL;
	
	return res;
}

bool tryParseArgList(struct Method* res, struct TokenList* copy, bool debug){
	
	if(!list_expect(copy, LPARENS)){
		return false;
	}

	//while there is no ')' up, continue parsing arguments
	struct Token* next = list_head(copy);
	
	if(next == NULL){
		return false;
	}

	while (next->kind != RPARENS) {
		if (res->count_args > 0) {
			if(!list_expect(copy, COMMA)){
				return false;
			}
		}
		
		struct DeclArg* da = makeDeclArg(copy, debug);
		if(da == NULL){
			return false;
		}
		res->args[res->count_args] = da;
		res->count_args++;
		
		res->args = realloc(res->args,sizeof(struct DeclArg*)*(res->count_args+1));

		next = list_head(copy);
		if(next == NULL){
			freeTokenListShallow(copy);
			return false;
		}
	}

	if(!list_expect(copy, RPARENS)){
		freeTokenListShallow(copy);
		return false;
	}
	
	return true;
}
