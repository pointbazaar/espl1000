#include <stdio.h>
#include <stdlib.h>

#include "BinConst.h"

#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../ast/ast.h"
#include "../../../../token/token.h"

struct BinConst* makeBinConst(struct TokenList* tokens, bool debug){
	
	if(debug){
		printf("BinConst(...) from ");
		list_print(tokens);
	}

	struct Token* tk = list_head(tokens);

	if(tk->kind != BINCONST){ return NULL; }


	struct BinConst* res = malloc(sizeof(struct BinConst));
	
	//"0b10" -> 2, ...
	//use strtol to convert bin string -> int
	res->value = strtol(tk->value_ptr+2, NULL, 2);
	
	list_consume(tokens, 1);

	return res;
}
