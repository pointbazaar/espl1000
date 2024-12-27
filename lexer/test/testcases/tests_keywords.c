#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../test.h"
#include "../token/token/token.h"
#include "../token/TokenKeys.h"
#include "../lexer_test_utils.h"

void test_if_else() {

	printt("test if else");

	char* str = "if else{ ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == IF);
	assert(tokens[1]->kind == ELSE);
	assert(tokens[2]->kind == LCURLY);

	free_tokens(tokens, 3);
}

void test_break() {

	printt("test break");

	char* str = "break; int x";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == KEYWORD_BREAK);
	assert(tokens[1]->kind == SEMICOLON);
	assert(tokens[2]->kind == TYPEID_PRIMITIVE_INT);

	free_tokens(tokens, 4);
}

void test_for() {

	printt("test_for");

	char* str = "for for{ ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == FOR);
	assert(tokens[1]->kind == FOR);
	assert(tokens[2]->kind == LCURLY);

	free_tokens(tokens, 3);
}

void test_in() {

	printt("test_in");

	char* str = "in; in{ ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == KEYWORD_IN);
	assert(tokens[1]->kind == SEMICOLON);
	assert(tokens[2]->kind == KEYWORD_IN);
	assert(tokens[3]->kind == LCURLY);

	free_tokens(tokens, 4);
}
