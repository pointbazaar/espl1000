#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "CallTest.h"

#include "statements/Call.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int methodcall_test1() {

	status_test("call test 1");

	struct TokenList* l = makeTokenList();

	list_add(l, makeToken2(ID, "main"));
	list_add(l, makeToken(LPARENS));
	list_add(l, makeToken(RPARENS));

	struct Call* call = makeCall(l);

	assert(0 == list_size(l));
	assert(call != NULL);

	freeTokenList(l);
	free_call(call);

	return 1;
}

int methodcall_test2() {

	status_test("call test 2");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID, "main"));
	list_add(list, makeToken(LPARENS));
	list_add(list, makeToken2(INTEGER, "4"));
	list_add(list, makeToken(RPARENS));

	struct Call* call = makeCall(list);

	assert(0 == list_size(list));
	assert(call != NULL);

	freeTokenList(list);
	free_call(call);

	return 1;
}

int methodcall_test3() {

	status_test("call test 3");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID, "main"));
	list_add(list, makeToken(LPARENS));
	list_add(list, makeToken2(ID, "x"));
	list_add(list, makeToken(RPARENS));

	struct Call* call = makeCall(list);

	assert(0 == list_size(list));
	assert(call != NULL);

	freeTokenList(list);
	free_call(call);

	return 1;
}

int methodcall_test_can_parse_subroutine_call() {

	status_test("call test can parse subroutine call");

	struct TokenList* tl = makeTokenList();
	list_add(tl, makeToken2(ID, "println"));
	list_add(tl, makeToken(LPARENS));
	list_add(tl, makeToken2(INTEGER, "1"));
	list_add(tl, makeToken(RPARENS));

	struct Call* call = makeCall(tl);

	assert(0 == list_size(tl));
	assert(call != NULL);

	freeTokenList(tl);
	free_call(call);

	return 1;
}

int methodcall_test_can_parse_subroutine_call2() {

	status_test("call test can parse subroutine call 2");

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID, "println"));
	list_add(tokens, makeToken2(LPARENS, "("));
	list_add(tokens, makeToken2(INTEGER, "5"));
	list_add(tokens, makeToken2(RPARENS, ")"));

	struct Call* call = makeCall(tokens);

	assert(0 == list_size(tokens));
	assert(call != NULL);

	freeTokenList(tokens);
	free_call(call);

	return 1;
}

int methodcall_test_can_parse_struct_member_access() {

	status_test("call test can parse struct member access");

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID, "x"));
	list_add(tokens, makeToken2(STRUCTMEMBERACCESS, "."));
	list_add(tokens, makeToken2(ID, "y"));
	list_add(tokens, makeToken2(LPARENS, "("));
	list_add(tokens, makeToken2(INTEGER, "3"));
	list_add(tokens, makeToken2(RPARENS, ")"));

	struct Call* call = makeCall(tokens);

	assert(0 == list_size(tokens));
	assert(call != NULL);

	assert(call->callable != NULL);

	assert(call->callable->simple_var != NULL);
	assert(call->callable->member_access != NULL);

	assert(strcmp(call->callable->simple_var->name, "x") == 0);
	assert(call->callable->simple_var->count_indices == 0);

	assert(strcmp(call->callable->member_access->simple_var->name, "y") == 0);

	freeTokenList(tokens);
	free_call(call);

	return 1;
}

int methodcall_test_can_parse_array_access() {

	status_test("call test can parse array access");

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID, "x"));
	list_add(tokens, makeToken2(LBRACKET, "["));
	list_add(tokens, makeToken2(ID, "y"));
	list_add(tokens, makeToken2(RBRACKET, "]"));
	list_add(tokens, makeToken2(LPARENS, "("));
	list_add(tokens, makeToken2(INTEGER, "1"));
	list_add(tokens, makeToken2(RPARENS, ")"));

	struct Call* call = makeCall(tokens);

	assert(0 == list_size(tokens));
	assert(call != NULL);

	assert(call->callable != NULL);

	assert(call->callable->simple_var != NULL);
	assert(call->callable->simple_var->count_indices == 1);

	assert(strcmp(call->callable->simple_var->name, "x") == 0);

	struct Expr* expr = call->callable->simple_var->indices[0];

	assert(strcmp(expr->term1->term->ptr.var_term->simple_var->name, "y") == 0);

	freeTokenList(tokens);
	free_call(call);

	return 1;
}
