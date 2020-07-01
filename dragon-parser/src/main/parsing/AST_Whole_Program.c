#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "AST_Whole_Program.h"
#include "Namespace.h"
#include "../commandline/TokenList.h"

struct AST_Whole_Program* makeAST_Whole_Program(struct Namespace* myNamespace) {
	//utility method, to facilitate creating AST_Whole_Program from a single namespace node

	struct AST_Whole_Program* res = malloc(sizeof(struct AST_Whole_Program));

	res->namespaces = malloc(sizeof(struct Namespace*)*1);

	res->namespaces[0] = myNamespace;

	return res;
}

struct AST_Whole_Program* makeAST_Whole_Program(struct TokenList* tokens, char* myNamespace, bool debug) {

	//use this constructor as a shortcut to create an AST
	//if there is just a single namespace

	if (debug) {
		printf("makeAST_Whole_Program(...)\n");
	}

	struct AST_Whole_Program* res = malloc(sizeof(struct AST_Whole_Program));

	res->namespaces = malloc(sizeof(struct Namespace*)*1);

	res->namespaces[0] = makeNamespace(tokens, myNamespace, debug);

	if(res->namespaces[0] == NULL){return NULL;}

	return res;
}

