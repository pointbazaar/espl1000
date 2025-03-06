#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "ParseExprTests.h"

#include "expr/Term.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int term_test_simple_term() {

	status_test("term_test_simple_term");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(INTEGER, "4"));

	struct Term* t = makeTerm(list);
	assert(t != NULL);

	freeTokenList(list);
	free_term(t);

	return 1;
}

int term_test_variable_term() {

	status_test("term_test_variable_term");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID, "x"));

	struct Term* t = makeTerm(list);
	assert(t != NULL);

	assert(list_size(list) == 0);

	struct Variable* v = t->ptr.m6;
	assert(v != NULL);

	struct SimpleVar* sv = v->simple_var;
	assert(sv != NULL);

	assert(strcmp(sv->name, "x") == 0);

	freeTokenList(list);
	free_term(t);

	return 1;
}

int term_test_parentheses() {

	status_test("term_test_parentheses");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(LPARENS, "("));
	list_add(list, makeToken2(ID, "x"));
	list_add(list, makeToken2(RPARENS, ")"));

	struct Term* t = makeTerm(list);
	assert(t != NULL);

	struct Expr* expr = t->ptr.m5;
	assert(expr != NULL);

	assert(list_size(list) == 0);

	struct Variable* v = expr->term1->term->ptr.m6;
	assert(v != NULL);

	struct SimpleVar* sv = v->simple_var;
	assert(sv != NULL);

	assert(strcmp(sv->name, "x") == 0);

	freeTokenList(list);
	free_term(t);

	return 1;
}
