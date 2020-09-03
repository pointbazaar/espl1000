#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SimpleType.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../util/util.h"
#include "../../../../ast/free_ast.h"
#include "../../../../token/token.h"

struct SimpleType* makeSimpleType2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("SimpleType(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) == 0){ return NULL; }

	struct SimpleType* res = smalloc(sizeof(struct SimpleType));
	
	strcpy(res->typeName, "");

	struct Token* token = list_head(tokens);

	if(debug){
		printf("\tinspect token kind\n");
	}
	
	if (token->kind == TYPEIDENTIFIER) {
		strcpy(res->typeName, token->value);
	} else if (token->kind == ANYTYPE) {
		strcpy(res->typeName, "#");
	} else {
		free(res);
		return NULL;
	}

	list_consume(tokens, 1);

	if(debug){
		printf("\tsuccess parsing SimpleType\n");
	}

	return res;
}

struct SimpleType* makeSimpleType(char* typeName) {
	struct SimpleType* res = smalloc(sizeof(struct SimpleType));

	strcpy(res->typeName, typeName);

	return res;
}

