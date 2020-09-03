#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "CharConst.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../../../../util/util.h"
#include "../../../../ast/free_ast.h"
#include "../../../../token/token.h"

struct CharConst* makeCharConst(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("CharConst(...)\n");
	}

	struct CharConst* res = smalloc(sizeof(struct CharConst));

	struct Token* token = list_head(tokens);
	
	if(token == NULL){
		free(res);
		return NULL;
	}

	if (token->kind == CCONST) {
		
		//e.g. in .$FILE.tokens
		//9 'h'
		
		//index needs to be 1, as charconst
		//is surrounded by single quotes
		res->value = token->value[1];
		
		list_consume(tokens, 1);
	} else {
		//"Error: could not read charConstant node";
		free(res);
		return NULL;
	}

	return res;
}
