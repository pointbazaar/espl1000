#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SimpleType.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/Token.h"
#include "../../commandline/TokenKeys.h"

struct SimpleType* makeSimpleType(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("SimpleType(...)\n");
	}

	struct SimpleType* res = malloc(sizeof(struct SimpleType));

	struct Token* token = list_get(tokens, 0);
	if(token == NULL){return NULL;}

	if (token->kind == TYPEIDENTIFIER) {
		res->typeName = token->value;
	} else if (token->kind == ANYTYPE) {
		res->typeName = "#";
	} else {
		//("Error: could not read simple type identifier");
		return NULL;
	}

	if(!list_consume(tokens, 1)){return NULL;}

	return res;
}

struct SimpleType* makeSimpleType(char* typeName) {
	struct SimpleType* res = malloc(sizeof(struct SimpleType));

	res->typeName = typeName;

	return res;
}
