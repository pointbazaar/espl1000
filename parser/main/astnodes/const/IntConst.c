#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include "IntConst.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

int32_t makeIntConst(struct TokenList* tokens, bool* error) {

	int32_t res;

	struct TokenList* copy = list_copy(tokens);

	struct Token* tk = list_head(copy);
	
	if(tk == NULL){
		*error = true;
		return 0;
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
					freeTokenListShallow(copy);
					*error = true;
					return 0;
				}
				
				res = - atoi(mytk->value_ptr);
				list_consume(copy, 2);
			} else {
				// "cannot parse integer constant node with such operator:" + tk->value;
				freeTokenListShallow(copy);
				*error = true;
				return 0;
			}
			break;

		case INTEGER: 
			;
			res = atoi(tk->value_ptr);
			list_consume(copy, 1);
			break;

		default:
			;
			// "could not read IntConst node";
			freeTokenListShallow(copy);
			*error = true;
			return 0;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}


