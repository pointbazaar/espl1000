#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "AST_Whole_Program.h"
#include "Namespace.h"
#include "../commandline/TokenList.h"
#include "../../../../util/util.h"

struct AST_Whole_Program* makeAST_Whole_Program(struct Namespace* myNamespace) {
	//utility method, to facilitate creating AST_Whole_Program from a single namespace node

	struct AST_Whole_Program* res = smalloc(sizeof(struct AST_Whole_Program));

	res->namespaces = malloc(sizeof(struct Namespace*)*1);

	res->namespaces[0] = myNamespace;

	return res;
}

struct AST_Whole_Program* makeAST_Whole_Program3(struct TokenList* tokens, char* myNamespace, bool debug) {

	//use this constructor as a shortcut to create an AST
	//if there is just a single namespace

	if (debug) {
		printf("makeAST_Whole_Program(...)\n");
	}

	struct AST_Whole_Program* res = smalloc(sizeof(struct AST_Whole_Program));

	res->namespaces = smalloc(sizeof(struct Namespace*)*1);

	res->namespaces[0] = makeNamespace(tokens, myNamespace, debug);

	if(res->namespaces[0] == NULL){
		free(res->namespaces);
		return NULL;
	}

	return res;
}

void freeAST_Whole_Program(struct AST_Whole_Program* ast){
	
	for(int i=0;i < ast->count_namespaces;i++){
		freeNamespace(ast->namespaces[i]);
	}
	free(ast->namespaces);
	free(ast);
}
