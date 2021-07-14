#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "BoolConst.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct BoolConst* makeBoolConst(struct TokenList* tokens) {

	if(list_size(tokens) == 0){
		return NULL;
	}
	
	struct BoolConst* res = make(BoolConst);
	
	struct Token* tk = list_head(tokens);
	
	res->super.line_num    = tk->line_num;
	res->super.annotations = 0;
	
	if (tk->kind == BCONST_TRUE) {
		
		res->value = true;
		list_consume(tokens, 1);
		
	}else if (tk->kind == BCONST_FALSE) {
		
		res->value = false;
		list_consume(tokens, 1);
		
	} else {
		free(res);
		return NULL;
	}

	return res;
}
