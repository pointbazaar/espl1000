#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "PrimitiveType.h"

#include "ast/ast.h"
#include "ast/sd_inttype.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct PrimitiveType* makePrimitiveType(struct TokenList* tokens, bool debug){

	struct PrimitiveType* res = make(PrimitiveType);
	
	res->super.line_num    = list_head(tokens)->line_num;
	res->super.annotations = 0;
	
	struct Token* next = list_head(tokens);
	
	res->intType = NONE;
	
	res->isIntType   = next->kind == TYPEID_PRIMITIVE_INT;
	res->isFloatType = next->kind == TYPEID_PRIMITIVE_FLOAT;
	res->isCharType  = next->kind == TYPEID_PRIMITIVE_CHAR;
	res->isBoolType  = next->kind == TYPEID_PRIMITIVE_BOOL;
	
	char* types[] = 
	{"int8","int16","int32","int64",
	"uint8","uint16","uint32","uint64",
	"int","uint"
	};
	
	if(!(res->isIntType)){
		
		list_consume(tokens, 1);
		return res;
	}
		
	for(int i = 0; i < 10; i++){
		
		if(strcmp(next->value_ptr, types[i]) == 0){
			res->intType = i;
			break;
		}
	}
	
	if(res->intType == NONE){
		printf("[Parser][Error]");
		printf("could not find the inttype\n");
		exit(1);
	}

	list_consume(tokens, 1);
	
	return res;
}
