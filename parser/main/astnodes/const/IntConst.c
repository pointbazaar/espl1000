#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "IntConst.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct IntConst* makeIntConst(struct TokenList* tokens, bool debug) {

	struct IntConst* res = make(IntConst);

	if(debug){
		printf("IntConst(...) from ");
		list_print(tokens);
	}

	struct TokenList* copy = list_copy(tokens);

	struct Token* tk = list_head(copy);
	if(tk == NULL){
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	switch (tk->kind){

		case OPKEY_ARITHMETIC: 
			;
			if (
				strcmp(tk->value_ptr,"-") == 0 
				&& (list_get(copy, 1)->kind == INTEGER)

			) {
				struct Token* mytk = list_get(copy,1);
				if(mytk == NULL){
					free(res);
					freeTokenListShallow(copy);
					return NULL;
				}
				
				res->value = - atoi(mytk->value_ptr);
				list_consume(copy, 2);
			} else {
				// "cannot parse integer constant node with such operator:" + tk->value;
				free(res);
				freeTokenListShallow(copy);
				return NULL;
			}
			break;

		case INTEGER: 
			;
			res->value = atoi(tk->value_ptr);
			list_consume(copy, 1);
			break;

		default:
			;
			// "could not read IntConst node";
			free(res);
			freeTokenListShallow(copy);
			return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}


