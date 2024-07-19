#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "ExprTest.h"

#include "expr/Expr.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

int expr_test_simple_expression() {

	status_test("expr_test_simple_expression");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(INTEGER, "4"));
	struct Expr* expr = makeExpr(list);

	assert(expr != NULL);
	assert(expr->term1 != NULL);
	assert(expr->term1->term->ptr.m12->ptr.m2_int_const == 4);

	freeTokenList(list);
	free_expr(expr);

	return 1;
}

int expr_test_variable_name_expression() {

	status_test("expr_test_variable_name_expression");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID, "x"));
	struct Expr* expr = makeExpr(list);

	assert(expr != NULL);
	assert(expr->term1->term->ptr.m6 != NULL);
	assert(expr->term1->term->ptr.m6->simple_var != NULL);
	assert(
	    strcmp(
		expr->term1->term->ptr.m6->simple_var->name,
		"x") == 0);

	freeTokenList(list);
	free_expr(expr);

	return 1;
}

int expr_recognize_2_op_expr() {

	status_test("expr_recognize_2_op_expr");

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(INTEGER, "1"));
	list_add(tokens, makeToken2(OPKEY_ARITHMETIC, "+"));
	list_add(tokens, makeToken2(INTEGER, "2"));

	list_add(tokens, makeToken2(LCURLY, "{"));

	struct Expr* expr = makeExpr(tokens);
	assert(expr != NULL);
	assert(list_size(tokens) == 1);

	freeTokenList(tokens);
	free_expr(expr);

	return 1;
}

int expr_test_comparison() {

	status_test("expr_test_comparison");

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken2(ID, "x"));
	list_add(l, makeToken2(OPKEY_RELATIONAL, "<"));
	list_add(l, makeToken2(INTEGER, "5"));

	list_add(l, makeToken2(LCURLY, "{"));

	struct Expr* expr = makeExpr(l);
	assert(expr != NULL);

	assert(list_size(l) == 1);

	struct Term* term1 = expr->term1->term;
	assert(term1 != NULL);

	struct Term* term2 = expr->term2->term;
	assert(term2 != NULL);

	assert(expr->op == OP_LT);

	//assert about the terms

	struct Variable* v = term1->ptr.m6;
	assert(v != NULL);

	assert(term2->ptr.m12->kind == 2);

	assert(term2->ptr.m12->ptr.m2_int_const == 5);

	struct SimpleVar* sv = v->simple_var;
	assert(sv != NULL);

	assert(strcmp(sv->name, "x") == 0);

	freeTokenList(l);
	free_expr(expr);

	return 1;
}

int expr_test_3_terms() {

	status_test("expr_test_3_terms");

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(INTEGER, "1"));
	list_add(tokens, makeToken2(OPKEY_ARITHMETIC, "+"));
	list_add(tokens, makeToken2(INTEGER, "2"));
	list_add(tokens, makeToken2(OPKEY_ARITHMETIC, "+"));
	list_add(tokens, makeToken2(INTEGER, "3"));

	list_add(tokens, makeToken2(LCURLY, "{"));

	struct Expr* expr = makeExpr(tokens);
	assert(expr != NULL);

	assert(expr->term1 != NULL);
	assert(expr->term2 != NULL);

	printf("op = %d", expr->op);
	fflush(stdout);
	assert(expr->op == OP_PLUS);

	assert(list_size(tokens) == 1);

	freeTokenList(tokens);
	free_expr(expr);

	return 1;
}
