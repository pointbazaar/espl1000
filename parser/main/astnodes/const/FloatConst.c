#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "FloatConst.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct FloatConst* makeFloatConst(struct TokenList* tokens, bool debug){

	if(list_size(tokens) == 0){ return NULL; }

	struct FloatConst* res = make(FloatConst);

	struct TokenList* copy = list_copy(tokens);

	struct Token* tk = list_head(copy);
	
	res->super.line_num    = tk->line_num;
	res->super.annotations = 0;

	res->value = 1.0;
	
	if(tk->kind == OPKEY_ARITHMETIC && strcmp(tk->value_ptr, "-") == 0){
		
		res->value = -1.0;
		
		list_consume(copy, 1);
	}

	if(list_size(copy) == 0){
		freeFloatConst(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	if(list_head(copy)->kind != FLOATING){
		freeFloatConst(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	res->value *= atof(list_head(copy)->value_ptr);
	list_consume(copy, 1);


	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

