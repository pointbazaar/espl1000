#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "SimpleVarTest.h"

#include "var/SimpleVar.h"
#include "expr/Expr.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int simplevar_test_parse_simple_variable() {

	status_test("simplevar_test_parse_simple_variable");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID,"x") );

	struct SimpleVar* node = makeSimpleVar(list);
	if(node==NULL){
		return 0;
	}
	assert(strcmp("x", node->name)==0);
	
	freeTokenList(list);
	free_simple_var(node);

	return 1;
}

int simplevar_test_parse_simple_indexed_variable() {

	status_test("simplevar_test_parse_simple_indexed_variable");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x")) ;
	list_add(list, makeToken2(LBRACKET,"[")) ;
	list_add(list, makeToken2(INTEGER,"1")) ;
	list_add(list, makeToken2(RBRACKET,"]")) ;


	struct SimpleVar* node = makeSimpleVar(list);
	assert(node != NULL);

	assert(strcmp("x", node->name) == 0);
	assert(node->count_indices == 1);

	assert(node->indices[0]->term1 != NULL);

	assert(
			1 == 
			(node->indices[0]->term1->term->ptr.m12->ptr.m2_int_const)	
	);
	
	freeTokenList(list);
	free_simple_var(node);

	return 1;
}

int simplevar_test_2_indices() {

	status_test("simplevar_test_2_indices");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x")) ;
	list_add(list, makeToken2(LBRACKET,"["));
	list_add(list, makeToken2(INTEGER,"1"));
	list_add(list, makeToken2(RBRACKET,"]"));
	list_add(list, makeToken2(LBRACKET,"["));
	list_add(list, makeToken2(INTEGER,"2"));
	list_add(list, makeToken2(RBRACKET,"]"));


	struct SimpleVar* node = makeSimpleVar(list);
	
	assert(node != NULL);
	assert(node->count_indices == 2);
	assert(node->indices[0] != NULL);
	assert(node->indices[1] != NULL);
	
	assert(node->indices[0]->term1 != NULL);
	assert(node->indices[0]->op == OP_NONE);
	assert(node->indices[0]->term2 == NULL);
	
	assert(node->indices[1]->term1 != NULL);
	assert(node->indices[1]->op == OP_NONE);
	assert(node->indices[1]->term2 == NULL);

	free_simple_var(node);
	freeTokenList(list);

	return 1;
}
