#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "ParseExprTests.h"

#include "expr/AddressOf.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int test_address_of() {

	status_test((char*)__func__);

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken(OPKEY_BITWISE_AND));
	list_add(list, makeToken2(ID, "x"));

	struct AddressOf* d = makeAddressOf2(list);
	assert(d != NULL);

	freeTokenList(list);
	free_address_of(d);

	return 1;
}
