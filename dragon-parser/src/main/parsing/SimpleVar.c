#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SimpleVar.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"
#include "../commandline/TokenList.h"
#include "Expr.h"

struct SimpleVar* makeSimpleVar(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("SimpleVar(...) from %s\n", list_code(tokens, debug));
	}

	struct SimpleVar* res = malloc(sizeof(struct SimpleVar));

	res->optIndex = NULL;

	struct TokenList* copy = list_copy(tokens);

	if(list_size(copy) == 0){ return NULL; }

	struct Token* token = list_head(copy);
	if(token == NULL){return NULL;}

	if (token->kind == ID) {
		res->name = token->value;
		if(list_size(copy) == 0){ return NULL; }
		list_consume(copy,1);

		//it could have an index
		if (list_size(copy) > 0 && list_head(copy)->kind == LBRACKET) {

			if(!list_expect(copy, LBRACKET)){return NULL;}
			res->optIndex = makeExpr(copy,debug);
			if(res->optIndex == NULL){return NULL;}

			if(!list_expect(copy, RBRACKET)){return NULL;}

		} else {
			res->optIndex = NULL;
			//pass, this assignment has no index to it
		}

	} else {
		printf("%s : %d : could not read variabe name. token was %s from context '%s'",
			tokens->relPath,
			token->lineNumber,
			token->value,
			list_code(tokens, debug)
		);

		return NULL;
	}
	
	if(debug){
		printf("sucess parsing SimpleVar\n");
	}

	list_set(tokens,copy);

	return res;
}

