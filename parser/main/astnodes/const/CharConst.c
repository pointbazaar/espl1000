#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "CharConst.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../ast/free_ast.h"
#include "../../../../token/token.h"

struct CharConst* makeCharConst(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("CharConst(...) from ");
		list_print(tokens);
	}

	struct Token* token = list_head(tokens);
	
	if(token == NULL){ return NULL; }

	if (token->kind != CCONST) {
		//"Error: could not read charConstant node";
		return NULL;
	}
	
	struct CharConst* res = malloc(sizeof(struct CharConst));	
	
	//index needs to be 1, as charconst
	//is surrounded by single quotes
	res->value = token->value_ptr[1];
	
	list_consume(tokens, 1);

	return res;
}
