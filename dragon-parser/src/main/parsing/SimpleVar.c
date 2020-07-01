#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SimpleVar.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"
#include "../commandline/TokenList.h"
#include "Expr.h"
#include "../commandline/smalloc.h"

struct SimpleVar* makeSimpleVar(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("SimpleVar(...) from %s\n", list_code(tokens));
	}

	struct SimpleVar* res = smalloc(sizeof(struct SimpleVar));

	res->indexOptional = NULL;

	struct TokenList* copy = makeTokenList(tokens);

	if(list_size(copy) == 0){
		//"Error: no tokens"
		return NULL;
	}

	struct Token* token = list_get(copy,0);
	if(token == NULL){return NULL;}

	if (token->kind == ID) {
		res->name = token->value;
		if(!list_consume(copy,1)){return NULL;}

		//it could have an index
		if (list_size(copy) > 0 && list_get(copy, 0)->kind == LBRACKET) {

			if(!list_expect(copy, LBRACKET)){return NULL;}
			res->indexOptional = makeExpr(copy,debug);
			if(res->indexOptional == NULL){return NULL;}

			if(!list_expect(copy, RBRACKET)){return NULL;}

		} else {
			res->indexOptional = NULL;
			//pass, this assignment has no index to it
		}

	} else {
		printf("%s : %d : could not read variabe name. token was %s from context '%s'",
			tokens->relPath,
			token->lineNumber,
			token->value,
			list_code(tokens)
		);

		return NULL;
	}

	list_set(tokens,copy);

	return res;
}

