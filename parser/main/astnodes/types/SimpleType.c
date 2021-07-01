#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "SimpleType.h"

#include "PrimitiveType.h"
#include "StructType.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct SimpleType* makeSimpleType(struct TokenList* tokens, bool debug) {

	if(list_size(tokens) == 0){ return NULL; }

	struct SimpleType* res = make(SimpleType);
	struct TokenList* copy = list_copy(tokens);
	
	parse_astnode(copy, &(res->super));
	
	res->primitiveType = NULL;
	res->structType    = NULL;
	
	struct Token* next = list_head(copy);
	
	switch(next->kind){
		
		case TYPEID_PRIMITIVE_INT:
		case TYPEID_PRIMITIVE_BOOL:
		case TYPEID_PRIMITIVE_CHAR:
		case TYPEID_PRIMITIVE_FLOAT:
		
			res->primitiveType = makePrimitiveType(copy, debug);
			break;
			
		case TYPEID:
		case ANYTYPE:
		
			res->structType = makeStructType(copy, debug);	
			break;
			
		default:
			free(res);
			freeTokenListShallow(copy);
			return NULL;
	}
	
	list_set(tokens, copy);
	freeTokenListShallow(copy);
	
	return res;	
}

