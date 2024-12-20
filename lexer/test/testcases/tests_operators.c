#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../test.h"
#include "../token/token/token.h"
#include "../token/TokenKeys.h"
#include "../lexer_test_utils.h"

void test_operators() {

	printt("test operators");

	char* str = "+ - * / ";
	struct Token** tokens = lex(str);

	assert(tokens != NULL);
	assert(tokens[0] != NULL);

	assert(tokens[0]->kind == OPKEY_ARITHMETIC_PLUS);
	assert(tokens[1]->kind == OPKEY_ARITHMETIC_MINUS);
	assert(tokens[2]->kind == OPKEY_ARITHMETIC_MUL);
	assert(tokens[3]->kind == OPKEY_ARITHMETIC_DIV);

	free_tokens(tokens, 4);
}

void test_operators_cmp() {

	printt("test operators_cmp");

	char* str = "<= >= == != < >";
	struct Token** tokens = lex(str);

	assert(tokens != NULL);
	assert(tokens[0] != NULL);

	assert(tokens[0]->kind == OPKEY_RELATIONAL_LE);
	assert(tokens[1]->kind == OPKEY_RELATIONAL_GE);
	assert(tokens[2]->kind == OPKEY_RELATIONAL_EQ);
	assert(tokens[3]->kind == OPKEY_RELATIONAL_NEQ);
	assert(tokens[4]->kind == OPKEY_RELATIONAL_LT);
	assert(tokens[5]->kind == OPKEY_RELATIONAL_GT);

	free_tokens(tokens, 6);
}

void test_operators_logical() {

	printt("test operators_logical");

	char* str = "&& || ";
	struct Token** tokens = lex(str);

	assert(tokens != NULL);
	assert(tokens[0] != NULL);

	assert(tokens[0]->kind == OPKEY_LOGICAL_AND);

	assert(tokens[1]->kind == OPKEY_LOGICAL_OR);

	free_tokens(tokens, 2);
}

void test_operators_bitwise() {

	printt("test operators_bitwise");

	char* str = "| & << >> ^ ~";
	struct Token** tokens = lex(str);

	assert(tokens != NULL);
	assert(tokens[0] != NULL);

	assert(tokens[0]->kind == OPKEY_BITWISE_OR);
	assert(tokens[1]->kind == OPKEY_BITWISE_AND);
	assert(tokens[2]->kind == OPKEY_BITWISE_SHIFT_LEFT);
	assert(tokens[3]->kind == OPKEY_BITWISE_SHIFT_RIGHT);
	assert(tokens[4]->kind == OPKEY_BITWISE_XOR);
	assert(tokens[5]->kind == OPKEY_BITWISE_NOT);

	free_tokens(tokens, 6);
}

void test_assign_operators() {

	printt("test_assign_operators");

	char* str = "= += -= >>= <<= &= |=";
	struct Token** tokens = lex(str);

	assert(tokens != NULL);
	assert(tokens[0] != NULL);

	// =
	assert(tokens[0]->kind == ASSIGNOP_SIMPLE);

	// arithmetic
	assert(tokens[1]->kind == ASSIGNOP_PLUS);
	assert(tokens[2]->kind == ASSIGNOP_MINUS);

	// bitwise
	assert(tokens[3]->kind == ASSIGNOP_SHIFT_RIGHT);
	assert(tokens[4]->kind == ASSIGNOP_SHIFT_LEFT);
	assert(tokens[5]->kind == ASSIGNOP_BITWISE_AND);
	assert(tokens[6]->kind == ASSIGNOP_BITWISE_OR);

	free_tokens(tokens, 7);
}
