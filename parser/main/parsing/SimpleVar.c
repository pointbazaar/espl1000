#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SimpleVar.h"
#include "Expr.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/TokenList.h"
#include "../../../token/token.h"
#include "../../../util/util.h"
#include "../../../ast/free_ast.h"

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
		strcpy(res->name, token->value);
		list_consume(copy,1);

		//it could have an index
		if (list_size(copy) > 0 
			&& list_head(copy)->kind == LBRACKET
		){

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
