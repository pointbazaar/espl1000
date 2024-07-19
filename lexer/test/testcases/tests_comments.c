#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../test.h"
#include "../../token/token/token.h"
#include "../../token/TokenKeys.h"
#include "../lexer_test_utils.h"

void test_single_line_comment() {

	printt("single line comment\n");

	char* str = "x1 //hello there \n x2 ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == ID);
	assert(tokens[1]->kind == ID);

	assert(strcmp(tokens[0]->value_ptr, "x1") == 0);
	assert(strcmp(tokens[1]->value_ptr, "x2") == 0);

	free_tokens(tokens, 2);
}

void test_multi_line_comment() {

	printt("multi line comment\n");

	char* str = "x1 /*hello //there \n now*/ x2 ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == ID);
	assert(tokens[1]->kind == ID);

	assert(strcmp(tokens[0]->value_ptr, "x1") == 0);
	assert(strcmp(tokens[1]->value_ptr, "x2") == 0);

	free_tokens(tokens, 2);
}
