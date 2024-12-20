#include <stdlib.h>
#include <assert.h>

#include "../../list/TokenList.h"
#include "../../token/token.h"

#include "../test.h"

void test_tokenlist_no_leak() {

	printt2("test tokenlist no leak\n");

	struct TokenList* list = makeTokenList();

	freeTokenList(list);
}

void test_tokenlist_1element() {

	printt2("test tokenlist 1 element\n");

	struct TokenList* list = makeTokenList();

	struct Token* tk = makeTokenStringConst("hello token");

	list_add(list, tk);

	freeTokenList(list);
}

void test_tokenlist_copy() {

	printt2("test tokenlist copy\n");

	struct TokenList* list = makeTokenList();

	struct Token* tk = makeTokenStringConst("hello token");

	list_add(list, tk);

	struct TokenList* copy = list_copy(list);

	freeTokenListShallow(copy);
	freeTokenList(list);
}

void test_tokenlist_code() {

	printt2("test list_code\n");

	struct TokenList* list = makeTokenList();

	struct Token* tk = makeTokenStringConst("hello token");

	list_add(list, tk);

	char* code = list_code(list);

	free(code);
	freeTokenList(list);
}

void test_tokenlist_print() {

	printt2("test list_print\n");

	struct TokenList* list = makeTokenList();

	struct Token* tk = makeTokenStringConst("hello token");

	list_add(list, tk);

	list_print(list);

	freeTokenList(list);
}

void test_tokenlist_consume() {

	printt2("test list_consume\n");

	struct TokenList* list = makeTokenList();

	struct Token* tk = makeTokenStringConst("hello token");

	list_add(list, tk);

	assert(list_size(list) == 1);

	list_consume(list, 1);

	assert(list_size(list) == 0);

	freeTokenList(list);
}
