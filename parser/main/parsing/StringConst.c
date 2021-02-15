#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "StringConst.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

struct StringConst* makeStringConst(struct TokenList* tokens, bool debug){
	
	if(debug){
		printf("StringConst(...) from ");
		list_print(tokens);
	}
	
	if(list_size(tokens) == 0){return NULL;}
	
	struct StringConst* res = 
		malloc(sizeof(struct StringConst));
	
	struct Token* head = list_head(tokens);
	
	if(head->kind == STRINGCONST){
		res->value = malloc
			(sizeof(char)*(strlen(head->value_ptr)+1));
			
		strcpy(res->value, head->value_ptr);
		
		list_consume(tokens, 1);
	}else{
		free(res);
		return NULL;
	}
	
	return res;
}
