#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../test.h"
#include "../token/token/token.h"
#include "../token/TokenKeys.h"
#include "../lexer_test_utils.h"

void test_plus_plus_minus_minus() {

	printt("test ++, --");

	char* str = "Char ++ -- ";
	//should expand: "Char += 1 -= 1"
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(count >= 3);
	assert(tokens != NULL);
	assert(tokens[0] != NULL);

	assert(tokens[0]->kind == TYPEID);
	assert(tokens[1]->kind == ASSIGNOP_PLUS);
	assert(tokens[2]->kind == INTEGER);
	assert(tokens[3]->kind == ASSIGNOP_MINUS);
	assert(tokens[4]->kind == INTEGER);

	assert(strcmp(tokens[2]->value_ptr, "1") == 0);
	assert(strcmp(tokens[4]->value_ptr, "1") == 0);

	free_tokens(tokens, 5);
}

void test_can_see_line_with_semicolon() {

	printt("test can see line with semicolon");

	char* str = "Char x; ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens != NULL);
	assert(tokens[0] != NULL);

	assert(tokens[0]->kind == TYPEID);
	assert(tokens[1]->kind == ID);
	assert(tokens[2]->kind == SEMICOLON);

	assert_eq(tokens[1]->value_ptr, "x");

	free_tokens(tokens, 3);
}

void test_can_see_line_with_operators() {

	printt("can see line with operators");

	char* str = "x = x+x; ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens != NULL);
	assert(tokens[0] != NULL);

	assert(tokens[0]->kind == ID);
	assert(tokens[1]->kind == ASSIGNOP_SIMPLE);
	assert(tokens[2]->kind == ID);
	assert(tokens[3]->kind == OPKEY_ARITHMETIC_PLUS);
	assert(tokens[4]->kind == ID);

	free_tokens(tokens, 6);
}

void test_lexes_return_statement_favorably() {

	printt("lexes return statement favorably");

	char* str = "return (-5)*n; ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens != NULL);
	assert(tokens[0] != NULL);

	assert(tokens[0]->kind == RETURN);

	assert(tokens[1]->kind == LPARENS);
	assert(tokens[2]->kind == OPKEY_ARITHMETIC_MINUS);
	assert(tokens[3]->kind == INTEGER);
	assert(tokens[4]->kind == RPARENS);

	assert(tokens[5]->kind == OPKEY_ARITHMETIC_MUL);
	assert(tokens[6]->kind == ID);
	assert(tokens[7]->kind == SEMICOLON);

	free_tokens(tokens, 8);
}

void test_lexes_other_return_statement() {

	printt("lexes other return statement");

	char* str = "return (n*faculty(n-1)); ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == RETURN);

	assert(tokens[1]->kind == LPARENS);
	assert(tokens[2]->kind == ID);
	assert(tokens[3]->kind == OPKEY_ARITHMETIC_MUL);
	assert(tokens[4]->kind == ID);

	assert(tokens[5]->kind == LPARENS);
	assert(tokens[6]->kind == ID);
	assert(tokens[7]->kind == OPKEY_ARITHMETIC_MINUS);
	assert(tokens[8]->kind == INTEGER);
	assert(tokens[9]->kind == RPARENS);
	assert(tokens[10]->kind == RPARENS);

	free_tokens(tokens, 12);
}

void test_identifier_1() {

	printt("test identifier token:1");

	char* str = "main ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == ID);
	assert(strcmp(tokens[0]->value_ptr, "main") == 0);

	free_tokens(tokens, 1);
}

void test_identifier_2() {

	printt("test identifier token:2");

	char* str = "arg_ls ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == ID);
	assert(strcmp(tokens[0]->value_ptr, "arg_ls") == 0);

	free_tokens(tokens, 1);
}

void test_struct() {

	printt("test struct token");

	char* str = "struct ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == STRUCT);

	free_tokens(tokens, 1);
}

void test_return() {

	printt("test return token");

	char* str = "return ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == RETURN);

	free_tokens(tokens, 1);
}

void test_string_1() {

	printt("test string token:1");

	char* str = "\"hi\" ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == STRINGCONST);

	assert_eq(tokens[0]->value_ptr, "hi");

	free_tokens(tokens, 1);
}

void test_typeidentifier() {

	printt("test typeidentifier token:1");

	char* str = "Carrot ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == TYPEID);
	assert(strcmp(tokens[0]->value_ptr, "Carrot") == 0);

	free_tokens(tokens, 1);
}

void test_typeidentifier_primitive() {

	printt("test typeidentifier primitive");

	char* str = "int uint int8 uint8 int16 uint16";

	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	int expect_kind[] = {
	    TYPEID_PRIMITIVE_INT,
	    TYPEID_PRIMITIVE_UINT,
	    TYPEID_PRIMITIVE_INT8,
	    TYPEID_PRIMITIVE_UINT8,
	    TYPEID_PRIMITIVE_INT16,
	    TYPEID_PRIMITIVE_UINT16,
	};

	size_t expect_count = 6;

	for (size_t i = 0; i < expect_count; i++) {
		assert(tokens[i]->kind == expect_kind[i]);
	}

	free_tokens(tokens, expect_count);

	//------------------------------
	char* str2 = "char bool ";

	struct Token** tokens2 = lex(str2, &count);

	assert(tokens2[0]->kind == TYPEID_PRIMITIVE_CHAR);
	assert(tokens2[1]->kind == TYPEID_PRIMITIVE_BOOL);

	free_tokens(tokens2, 2);
}

void test_typeparameter_1() {

	printt("test typeparameter token");

	char* str = "?T0 ?T1 (?T2";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == TPARAM);
	assert(tokens[1]->kind == TPARAM);
	assert(tokens[2]->kind == LPARENS);
	assert(tokens[3]->kind == TPARAM);
	assert(strcmp(tokens[0]->value_ptr, "0") == 0);
	free_tokens(tokens, 4);
}

void test_comma() {

	printt("test comma token");

	char* str = ",, ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == COMMA);
	assert(tokens[1]->kind == COMMA);

	free_tokens(tokens, 2);
}

void test_arrow() {

	printt("test arrow token");

	char* str = "-> ~> ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == ARROW_NO_SIDE_EFFECT);
	assert(tokens[1]->kind == ARROW_SIDE_EFFECT);

	free_tokens(tokens, 2);
}

void test_rangeop_1() {

	printt("test_rangeop_1");

	char* str = ".. a.... ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == RANGEOP);
	assert(tokens[1]->kind == ID);
	assert(tokens[2]->kind == RANGEOP);
	assert(tokens[3]->kind == RANGEOP);

	free_tokens(tokens, 4);
}

void test_rangeop_2() {

	printt("test_rangeop_1");

	char* str = "0 .. 3 a .. b ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == INTEGER);
	assert(tokens[1]->kind == RANGEOP);
	assert(tokens[2]->kind == INTEGER);

	assert(tokens[3]->kind == ID);
	assert(tokens[4]->kind == RANGEOP);
	assert(tokens[5]->kind == ID);

	free_tokens(tokens, 6);
}

void test_member_access() {

	printt("test_member_access");

	char* str = "s.member= ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == ID);
	assert(tokens[1]->kind == STRUCTMEMBERACCESS);
	assert(tokens[2]->kind == ID);

	assert(tokens[3]->kind == ASSIGNOP_SIMPLE);

	free_tokens(tokens, 4);
}

void test_brackets() {

	printt("test_brackets");

	char* str = "[](){}< > ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == LBRACKET);
	assert(tokens[1]->kind == RBRACKET);
	assert(tokens[2]->kind == LPARENS);
	assert(tokens[3]->kind == RPARENS);
	assert(tokens[4]->kind == LCURLY);
	assert(tokens[5]->kind == RCURLY);
	assert(tokens[6]->kind == OPKEY_RELATIONAL_LT);
	assert(tokens[7]->kind == OPKEY_RELATIONAL_GT);

	free_tokens(tokens, 8);
}
