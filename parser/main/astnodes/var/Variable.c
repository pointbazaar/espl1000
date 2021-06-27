#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Variable.h"
#include "SimpleVar.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct Variable* makeVariable(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Variable(...) from ");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 1){ return NULL; }

	struct Variable* res = make(Variable);
	
	res->super.line_num    = list_head(tokens)->line_num;
	res->super.annotations = 0;

	res->simpleVar = NULL;
	res->memberAccess = NULL;

	struct TokenList* copy = list_copy(tokens);

	res->simpleVar = makeSimpleVar(copy,debug);

	if(res->simpleVar == NULL){
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	if (list_size(copy) < 2) { goto end; }
	
	struct Token* next = list_head(copy);

	if(next->kind != STRUCTMEMBERACCESS){ goto end; }

	list_expect(copy, STRUCTMEMBERACCESS);

	res->memberAccess = makeVariable(copy, debug);

	if(res->memberAccess == NULL){

		printf("Error parsing struct member access!\n");
		printf("%s\n", list_code(copy, debug));
		free(res);
		freeTokenListShallow(copy);
		exit(1);
	}
	
	end:

	if(debug){ printf("sucess parsing Variable\n"); }

	list_set(tokens,copy);
	freeTokenListShallow(copy);

	return res;
}

