#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Op.h"

#include "ast/util/free_ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

struct Op* makeOp(struct TokenList* tokens, bool debug){

	if(debug){
		printf("Op(...) from ");
		list_print(tokens);
	}

	struct TokenList* copy = list_copy(tokens);

	struct Token* tkn = list_head(copy);
	
	if(tkn == NULL){ return NULL; }

	if(tkn->kind != OPKEY){
		
		freeTokenListShallow(copy);
		return NULL;
	}
	
	struct Op* res = make(Op);
	memset(res, 0, sizeof(struct Op));
		
	strcpy(res->op, tkn->value_ptr);
	
	list_consume(copy, 1);
	
	list_set(tokens,copy);
	freeTokenListShallow(copy);

	return res;
}

