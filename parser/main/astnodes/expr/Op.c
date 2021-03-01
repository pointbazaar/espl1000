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

	struct Op* res = malloc(sizeof(struct Op));
	memset(res, 0, sizeof(struct Op));

	struct TokenList* copy = list_copy(tokens);

	struct Token* tkn = list_head(copy);
	
	if(tkn == NULL){
		free(res);
		return NULL;
	}

	if(tkn->kind == OPKEY){
		
		strcpy(res->op, tkn->value_ptr);
		
		list_consume(copy, 1);
	
	}else{
		//"could not recognize operator, got : " + tkn->value;
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing Op\n");
	}

	list_set(tokens,copy);
	freeTokenListShallow(copy);

	return res;
}

