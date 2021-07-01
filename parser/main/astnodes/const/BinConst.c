#include <stdio.h>
#include <stdlib.h>

#include "BinConst.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct BinConst* makeBinConst(struct TokenList* tokens, bool debug){

	struct Token* tk = list_head(tokens);

	if(tk->kind != BINCONST){ return NULL; }

	struct BinConst* res = make(BinConst);
	
	res->super.line_num    = tk->line_num;
	res->super.annotations = 0;
	
	//"0b10" -> 2, ...
	//use strtol to convert bin string -> int
	res->value = strtol(tk->value_ptr+2, NULL, 2);
	
	list_consume(tokens, 1);

	return res;
}
