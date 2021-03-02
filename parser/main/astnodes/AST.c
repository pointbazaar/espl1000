#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "AST.h"
#include "Namespace.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

#include "ast/util/free_ast.h"

struct AST* makeAST(struct Namespace* myNamespace) {
	//utility method, to facilitate creating AST_Whole_Program from a single namespace node

	struct AST* res = malloc(sizeof(struct AST));

	res->namespaces = malloc(sizeof(struct Namespace*)*1);

	res->namespaces[0] = myNamespace;

	return res;
}

struct AST* makeAST3(struct TokenList* tokens, char* myNamespace, bool debug) {

	//use this constructor as a shortcut to create an AST
	//if there is just a single namespace

	if (debug) {
		printf("makeAST_Whole_Program(...)\n");
	}

	struct AST* res = malloc(sizeof(struct AST));

	res->namespaces = malloc(sizeof(struct Namespace*)*1);

	res->namespaces[0] = makeNamespace(tokens, myNamespace, debug);

	if(res->namespaces[0] == NULL){
		free(res->namespaces);
		return NULL;
	}

	return res;
}
