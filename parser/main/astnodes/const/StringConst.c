#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "StringConst.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct StringConst* makeStringConst(struct TokenList* tokens, bool debug){
	
	if(debug){
		printf("StringConst(...) from ");
		list_print(tokens);
	}
	
	if(list_size(tokens) == 0){return NULL;}
	
	struct StringConst* res = make(StringConst);
	
	struct Token* tk = list_head(tokens);
	
	res->super.line_num    = tk->line_num;
	res->super.annotations = 0;
	
	if(tk->kind == STRINGCONST){
		res->value = malloc
			(sizeof(char)*(strlen(tk->value_ptr)+1));
			
		strcpy(res->value, tk->value_ptr);
		
		list_consume(tokens, 1);
	}else{
		free(res);
		return NULL;
	}
	
	return res;
}
