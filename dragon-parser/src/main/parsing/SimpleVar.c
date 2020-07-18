#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SimpleVar.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"
#include "../commandline/TokenList.h"
#include "Expr.h"
#include "../../../../util/util.h"

struct SimpleVar* makeSimpleVar(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("SimpleVar(...) from ");
		list_print(tokens);
	}
	
	if(list_size(tokens) == 0){ 
		return NULL; 
	}

	struct SimpleVar* res = smalloc(sizeof(struct SimpleVar));

	res->optIndex = NULL;

	struct TokenList* copy = list_copy(tokens);

	struct Token* token = list_head(copy);

	if (token->kind == ID) {
		res->name = token->value;
		list_consume(copy,1);

		//it could have an index
		if (list_size(copy) > 0 && list_head(copy)->kind == LBRACKET) {

			if(!list_expect(copy, LBRACKET)){
				free(res);
				freeTokenListShallow(copy);
				return NULL;
			}
			res->optIndex = makeExpr(copy,debug);
			if(res->optIndex == NULL){
				free(res);
				freeTokenListShallow(copy);
				return NULL;
			}

			if(!list_expect(copy, RBRACKET)){
				free(res);
				freeTokenListShallow(copy);
				return NULL;
			}

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
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing SimpleVar\n");
	}

	list_set(tokens,copy);
	freeTokenListShallow(copy);

	return res;
}

void freeSimpleVar(struct SimpleVar* sv){
	
	if(sv->optIndex != NULL){
		freeExpr(sv->optIndex);
	}
	free(sv);
}

