#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SimpleType.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/Token.h"
#include "../../commandline/TokenKeys.h"

struct SimpleType* makeSimpleType2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("SimpleType(...) from: %s\n", list_code(tokens));
	}

	struct SimpleType* res = malloc(sizeof(struct SimpleType));
	if(res == NULL){return NULL;}

	if(list_size(tokens) == 0){return NULL;}

	struct Token* token = list_head(tokens);
	if(token == NULL){return NULL;}

	if (token->kind == TYPEIDENTIFIER) {
		res->typeName = token->value;
	} else if (token->kind == ANYTYPE) {
		res->typeName = "#";
	} else {
		if(debug){
			printf("Error: could not read simple type identifier\n");
		}
		return NULL;
	}

	list_consume(tokens, 1);

	return res;
}

struct SimpleType* makeSimpleType(char* typeName) {
	struct SimpleType* res = malloc(sizeof(struct SimpleType));
	if(res == NULL){return NULL;}

	res->typeName = typeName;

	return res;
}
