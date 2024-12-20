#include <stdlib.h>
#include <assert.h>

#include "../../list/TokenList.h"
#include "../../token/token.h"

#include "../test.h"

#include "util/status/status.h"

void test_tokenlist_no_leak() {

	status("test tokenlist no leak");

	struct TokenList* list = makeTokenList();

	freeTokenList(list);
}

void test_tokenlist_1element() {

	status("test tokenlist 1 element");

	struct TokenList* list = makeTokenList();

	struct Token* tk = makeTokenStringConst("hello token");

	list_add(list, tk);

	freeTokenList(list);
}

void test_tokenlist_copy() {

	status("test tokenlist copy");

	struct TokenList* list = makeTokenList();

	struct Token* tk = makeTokenStringConst("hello token");

	list_add(list, tk);

	struct TokenList* copy = list_copy(list);

	freeTokenListShallow(copy);
	freeTokenList(list);
}

void test_tokenlist_code() {

	status("test list_code");

	struct TokenList* list = makeTokenList();

	struct Token* tk = makeTokenStringConst("hello token");

	list_add(list, tk);

	char* code = list_code(list);

	free(code);
	freeTokenList(list);
}

void test_tokenlist_print() {

	status("test list_print");

	struct TokenList* list = makeTokenList();

	struct Token* tk = makeTokenStringConst("hello token");

	list_add(list, tk);

	list_print(list);

	freeTokenList(list);
}

void test_tokenlist_consume() {

	status("test list_consume");

	struct TokenList* list = makeTokenList();

	struct Token* tk = makeTokenStringConst("hello token");

	list_add(list, tk);

	assert(list_size(list) == 1);

	list_consume(list, 1);

	assert(list_size(list) == 0);

	freeTokenList(list);
}
