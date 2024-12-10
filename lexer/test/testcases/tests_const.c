#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../test.h"
#include "../token/token/token.h"
#include "../token/TokenKeys.h"
#include "../lexer_test_utils.h"

void test_lexes_escaped_char() {

	printt("lexes escaped char\n");

	char* str = "return '\\n'";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == RETURN);
	assert(tokens[1]->kind == CCONST);
	assert(strcmp(tokens[1]->value_ptr, "'\\n'") == 0);

	free_tokens(tokens, 2);
}

void test_anytypetoken() {

	printt("test anytype token\n");

	char* str = "# ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == ANYTYPE);

	free_tokens(tokens, 1);
}

void test_true() {

	printt("test boolconst token\n");

	char* str = "true ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == BCONST_TRUE);

	free_tokens(tokens, 1);
}

void test_false() {

	printt("test boolconst token\n");

	char* str = "false ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == BCONST_FALSE);

	free_tokens(tokens, 1);
}

void test_char() {

	printt("test charconst token\n");

	char* str = "'x' ('\\n') 'h' ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == CCONST);
	assert(tokens[1]->kind == LPARENS);
	assert(tokens[2]->kind == CCONST);
	assert(tokens[3]->kind == RPARENS);

	assert(strcmp(tokens[0]->value_ptr, "'x'") == 0);

	assert(strcmp(tokens[2]->value_ptr, "'\\n'") == 0);

	assert(tokens[4]->kind == CCONST);
	assert(strcmp(tokens[4]->value_ptr, "'h'") == 0);

	free_tokens(tokens, 5);
}

void test_int_1() {

	printt("test integer token:1\n");

	char* str = "2038 ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == INTEGER);
	assert(strcmp(tokens[0]->value_ptr, "2038") == 0);

	free_tokens(tokens, 1);
}

void test_int_2() {

	printt("test integer token:2\n");

	char* str = "0 ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == INTEGER);
	assert(strcmp(tokens[0]->value_ptr, "0") == 0);

	free_tokens(tokens, 1);
}

void test_int_3() {

	printt("test integer token:3\n");

	char* str = "-5 ";
	struct Token** tokens = lex(str);

	assert(tokens[1]->kind == INTEGER);
	assert(strcmp(tokens[1]->value_ptr, "5") == 0);

	free_tokens(tokens, 2);
}

void test_hex() {

	printt("test hex\n");

	char* str = "0x1 0x0 0x10 0x1f 0xA3";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == HEXCONST);
	assert(strcmp(tokens[0]->value_ptr, "0x1") == 0);

	assert(tokens[1]->kind == HEXCONST);
	assert(strcmp(tokens[1]->value_ptr, "0x0") == 0);

	assert(tokens[2]->kind == HEXCONST);
	assert(strcmp(tokens[2]->value_ptr, "0x10") == 0);

	assert(tokens[3]->kind == HEXCONST);
	assert(strcmp(tokens[3]->value_ptr, "0x1f") == 0);

	assert(tokens[4]->kind == HEXCONST);
	assert(strcmp(tokens[4]->value_ptr, "0xA3") == 0);

	free_tokens(tokens, 5);
}

void test_binconst() {

	printt("test binconst\n");

	char* str = "0b0 0b1 0b10 0b101";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == BINCONST);
	assert(strcmp(tokens[0]->value_ptr, "0b0") == 0);

	assert(tokens[1]->kind == BINCONST);
	assert(strcmp(tokens[1]->value_ptr, "0b1") == 0);

	assert(tokens[2]->kind == BINCONST);
	assert(strcmp(tokens[2]->value_ptr, "0b10") == 0);

	assert(tokens[3]->kind == BINCONST);
	assert(strcmp(tokens[3]->value_ptr, "0b101") == 0);

	free_tokens(tokens, 4);
}
