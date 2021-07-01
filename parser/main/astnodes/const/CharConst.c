#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "CharConst.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct CharConst* makeCharConst(struct TokenList* tokens, bool debug) {

	struct Token* tk = list_head(tokens);
	
	if(tk == NULL){ return NULL; }

	if (tk->kind != CCONST) {
		//"Error: could not read charConstant node";
		return NULL;
	}
	
	struct CharConst* res = make(CharConst);	
	
	res->super.line_num    = tk->line_num;
	res->super.annotations = 0;
	
	//index needs to be 1, as charconst
	//is surrounded by single quotes
	res->value = tk->value_ptr[1];
	
	list_consume(tokens, 1);

	return res;
}
