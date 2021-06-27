#include <stdio.h>
#include <stdlib.h>

#include "HexConst.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct HexConst* makeHexConst(struct TokenList* tokens, bool debug){
	
	if(debug){
		printf("HexConst(...) from ");
		list_print(tokens);
	}

	struct Token* tk = list_head(tokens);

	if(tk->kind != HEXCONST){ return NULL; }

	struct HexConst* res = make(HexConst);
	
	res->super.line_num    = tk->line_num;
	res->super.annotations = 0;
	
	//"0x10" -> 16, ...
	//use strtol to convert hex string -> int
	res->value = strtol(tk->value_ptr+2, NULL, 16);
	
	list_consume(tokens, 1);
	
	return res;
}
