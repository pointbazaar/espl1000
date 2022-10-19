#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "PrimitiveType.h"

#include "ast/ast.h"
#include "ast/sd_inttype.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct PrimitiveType* makePrimitiveType(struct TokenList* tokens) {

	struct PrimitiveType* res = make(PrimitiveType);
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));
	
	struct Token* next = list_head(copy);
	
	res->int_type = NONE;
	
	res->is_int_type   = next->kind == TYPEID_PRIMITIVE_INT;
	res->is_char_type  = next->kind == TYPEID_PRIMITIVE_CHAR;
	res->is_bool_type  = next->kind == TYPEID_PRIMITIVE_BOOL;
	
	char* types[] = 
	{"int8",
	"uint8",
	"int",
	"uint"
	};
	
	if(!(res->is_int_type)){
		
		list_consume(copy, 1);
		
		list_set(tokens, copy);
		freeTokenListShallow(copy);
		
		return res;
	}
		
	for(int i = 0; i < 10; i++){
		
		if(strcmp(next->value_ptr, types[i]) == 0){
			res->int_type = i;
			break;
		}
	}
	
	if(res->int_type == NONE){
		printf("[Parser][Error]");
		printf("could not find the inttype\n");
		exit(1);
	}

	list_consume(copy, 1);
	
	list_set(tokens, copy);
	freeTokenListShallow(copy);
	
	return res;
}
