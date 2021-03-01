#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "SimpleVarTest.h"

#include "var/SimpleVar.h"
#include "const/IntConst.h"
#include "expr/Expr.h"
#include "expr/Term.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

#include "ast/util/free_ast.h"

int simplevar_test_parse_simple_variable(bool debug) {

	if(debug){
		printf("TEST: simplevar_test_parse_simple_variable()\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID,"x") );

	struct SimpleVar* node = makeSimpleVar(list,debug);
	if(node==NULL){
		return 0;
	}
	assert(strcmp("x", node->name)==0);
	
	freeTokenList(list);
	freeSimpleVar(node);

	return 1;
}

int simplevar_test_parse_simple_indexed_variable(bool debug) {

	if(debug){
		printf("TEST: simplevar_test_parse_simple_indexed_variable()\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x")) ;
	list_add(list, makeToken2(LBRACKET,"[")) ;
	list_add(list, makeToken2(INTEGER,"1")) ;
	list_add(list, makeToken2(RBRACKET,"]")) ;


	struct SimpleVar* node = makeSimpleVar(list,debug);
	assert(node != NULL);

	assert(strcmp("x", node->name) == 0);
	assert(node->count_indices == 1);

	assert(node->indices[0]->term1 != NULL);
	assert(node->indices[0]->term1->term->ptr.m2 != NULL);

	assert(
			1 == 
			(node->indices[0]->term1->term->ptr.m2)	//IntConst
			->value
	);
	
	freeTokenList(list);
	freeSimpleVar(node);

	return 1;
}

int simplevar_test_2_indices(bool debug){
	
	if(debug){
		printf("TEST: simplevar_test_2_indices()\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x")) ;
	list_add(list, makeToken2(LBRACKET,"["));
	list_add(list, makeToken2(INTEGER,"1"));
	list_add(list, makeToken2(RBRACKET,"]"));
	list_add(list, makeToken2(LBRACKET,"["));
	list_add(list, makeToken2(INTEGER,"2"));
	list_add(list, makeToken2(RBRACKET,"]"));


	struct SimpleVar* node = makeSimpleVar(list,debug);
	
	assert(node != NULL);
	assert(node->count_indices == 2);
	assert(node->indices[0] != NULL);
	assert(node->indices[1] != NULL);
	
	assert(node->indices[0]->term1 != NULL);
	assert(node->indices[0]->op == NULL);
	assert(node->indices[0]->term2 == NULL);
	
	assert(node->indices[1]->term1 != NULL);
	assert(node->indices[1]->op == NULL);
	assert(node->indices[1]->term2 == NULL);
	
	freeSimpleVar(node);

	return 1;
}
