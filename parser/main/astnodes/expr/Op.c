#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser/main/util/parse_astnode.h"

#include "Op.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct Op* makeOp(struct TokenList* tokens, bool debug){

	struct TokenList* copy = list_copy(tokens);

	struct Token* tk = list_head(copy);
	if(tk == NULL){ return NULL; }
	
	struct Op* res = make(Op);
	memset(res, 0, sizeof(struct Op));
	
	parse_astnode(copy, &(res->super));

	switch(tk->kind){
		
		case OPKEY_ARITHMETIC: res->is_arithmetic = true; break;
		case OPKEY_RELATIONAL: res->is_relational = true; break;
		case OPKEY_LOGICAL:    res->is_logical    = true; break;
		case OPKEY_BITWISE:    res->is_bitwise    = true; break;
		
		default:
			freeTokenListShallow(copy);
			free(res);
			return NULL;
	}
		
	strcpy(res->op, tk->value_ptr);
	
	list_consume(copy, 1);
	
	list_set(tokens,copy);
	freeTokenListShallow(copy);

	return res;
}

