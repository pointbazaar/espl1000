
#include <assert.h>
#include "MDirectTest.h"
#include "parser/test/test_parser_util.h"
#include "token/list/TokenList.h"
#include "token/token/token.h"
#include "token/TokenKeys.h"
#include "expr/MDirect.h"
#include "ast/util/free_ast.h"

bool mdirect_test_1() {

	status_test("mdirect_test_1");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken(LBRACKET));
	list_add(list, makeToken2(ID, "yx"));
	list_add(list, makeToken(RBRACKET));

	struct MDirect* m = makeMDirect(list);

	struct Expr* expr = m->expr;
	assert(expr != NULL);

	assert(list_size(list) == 0);

	struct Variable* v = expr->term1->term->ptr.m6;
	assert(v != NULL);

	struct SimpleVar* sv = v->simple_var;
	assert(sv != NULL);

	assert(strcmp(sv->name, "yx") == 0);

	freeTokenList(list);
	free_mdirect(m);

	return 1;
}