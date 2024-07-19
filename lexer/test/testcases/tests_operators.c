#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../test.h"
#include "../../token/token/token.h"
#include "../../token/TokenKeys.h"
#include "../lexer_test_utils.h"

void test_operators() {

	printt("test operators\n");

	char* str = "+ - * ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == OPKEY_ARITHMETIC);
	assert(strcmp(tokens[0]->value_ptr, "+") == 0);

	assert(tokens[1]->kind == OPKEY_ARITHMETIC);
	assert(strcmp(tokens[1]->value_ptr, "-") == 0);

	assert(tokens[2]->kind == OPKEY_ARITHMETIC);
	assert(strcmp(tokens[2]->value_ptr, "*") == 0);

	free_tokens(tokens, 3);
}

void test_operators_cmp() {

	printt("test operators_cmp\n");

	char* str = "<= >= == != < >";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == OPKEY_RELATIONAL);
	assert(strcmp(tokens[0]->value_ptr, "<=") == 0);

	assert(tokens[1]->kind == OPKEY_RELATIONAL);
	assert(strcmp(tokens[1]->value_ptr, ">=") == 0);

	assert(tokens[2]->kind == OPKEY_RELATIONAL);
	assert(strcmp(tokens[2]->value_ptr, "==") == 0);

	assert(tokens[3]->kind == OPKEY_RELATIONAL);
	assert(strcmp(tokens[3]->value_ptr, "!=") == 0);

	assert(tokens[4]->kind == OPKEY_RELATIONAL);
	assert(strcmp(tokens[4]->value_ptr, "<") == 0);

	assert(tokens[5]->kind == OPKEY_RELATIONAL);
	assert(strcmp(tokens[5]->value_ptr, ">") == 0);

	free_tokens(tokens, 6);
}

void test_operators_logical() {

	printt("test operators_logical\n");

	char* str = "&& || ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == OPKEY_LOGICAL);
	assert(strcmp(tokens[0]->value_ptr, "&&") == 0);

	assert(tokens[1]->kind == OPKEY_LOGICAL);
	assert(strcmp(tokens[1]->value_ptr, "||") == 0);

	free_tokens(tokens, 2);
}

void test_operators_bitwise() {

	printt("test operators_bitwise\n");

	char* str = "| & << >> ^ ~";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == OPKEY_BITWISE);
	assert(strcmp(tokens[0]->value_ptr, "|") == 0);

	assert(tokens[1]->kind == OPKEY_BITWISE);
	assert(strcmp(tokens[1]->value_ptr, "&") == 0);

	assert(tokens[2]->kind == OPKEY_BITWISE);
	assert(strcmp(tokens[2]->value_ptr, "<<") == 0);

	assert(tokens[3]->kind == OPKEY_BITWISE);
	assert(strcmp(tokens[3]->value_ptr, ">>") == 0);

	assert(tokens[4]->kind == OPKEY_BITWISE);
	assert(strcmp(tokens[4]->value_ptr, "^") == 0);

	assert(tokens[5]->kind == OPKEY_BITWISE);
	assert(strcmp(tokens[5]->value_ptr, "~") == 0);

	free_tokens(tokens, 6);
}

void test_assign_operators() {

	printt("test_assign_operators\n");

	char* str = "= += -= >>= <<= &= |=";
	struct Token** tokens = lex(str);

	// =
	assert(tokens[0]->kind == ASSIGNOP);
	assert(strcmp(tokens[0]->value_ptr, "=") == 0);

	// arithmetic
	assert(tokens[1]->kind == ASSIGNOP);
	assert(strcmp(tokens[1]->value_ptr, "+=") == 0);

	assert(tokens[2]->kind == ASSIGNOP);
	assert(strcmp(tokens[2]->value_ptr, "-=") == 0);

	// bitwise
	assert(tokens[3]->kind == ASSIGNOP);
	assert(strcmp(tokens[3]->value_ptr, ">>=") == 0);

	assert(tokens[4]->kind == ASSIGNOP);
	assert(strcmp(tokens[4]->value_ptr, "<<=") == 0);

	assert(tokens[5]->kind == ASSIGNOP);
	assert(strcmp(tokens[5]->value_ptr, "&=") == 0);

	assert(tokens[6]->kind == ASSIGNOP);
	assert(strcmp(tokens[6]->value_ptr, "|=") == 0);

	free_tokens(tokens, 7);
}
