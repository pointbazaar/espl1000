#include <stdio.h>
#include <stdlib.h>

#include "HexConst.h"

#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../ast/ast.h"
#include "../../../../token/token.h"

struct HexConst* makeHexConst(struct TokenList* tokens, bool debug){
	
	if(debug){
		printf("HexConst(...) from ");
		list_print(tokens);
	}

	struct Token* tk = list_head(tokens);

	if(tk->kind != HEXCONST){ return NULL; }


	struct HexConst* res = malloc(sizeof(struct HexConst));
	
	//"0x10" -> 16, ...
	//use strtol to convert hex string -> int
	res->value = strtol(tk->value_ptr+2, NULL, 16);
	
	list_consume(tokens, 1);
	
	return res;
}
