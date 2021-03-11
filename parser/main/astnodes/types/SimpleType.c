#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SimpleType.h"

#include "PrimitiveType.h"
#include "StructType.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct SimpleType* makeSimpleType(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("SimpleType(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) == 0){ return NULL; }

	struct SimpleType* res = make(SimpleType);
	
	res->primitiveType = NULL;
	res->structType    = NULL;
	
	struct Token* next = list_head(tokens);
	
	switch(next->kind){
		
		case TYPEID_PRIMITIVE_INT:
		case TYPEID_PRIMITIVE_BOOL:
		case TYPEID_PRIMITIVE_CHAR:
		case TYPEID_PRIMITIVE_FLOAT:
		
			res->primitiveType = makePrimitiveType(tokens, debug);
			break;
			
		case TYPEID:
		case ANYTYPE:
		
			res->structType = makeStructType(tokens, debug);	
			break;
			
		default:
			free(res);
			return NULL;
	}
	
	return res;	
}

