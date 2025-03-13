#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../test.h"
#include "../token/token/token.h"
#include "../token/TokenKeys.h"
#include "../lexer_test_utils.h"

void test_mixed_1() {

	printt("test mixed 1");

	char* str = "struct MyStruct { uint a, MyStruct b} ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[2]->kind == LCURLY);

	assert(tokens[3]->kind == TYPEID_PRIMITIVE_UINT);

	assert(tokens[4]->kind == ID);
	assert(strcmp(tokens[4]->value_ptr, "a") == 0);

	assert(tokens[5]->kind == COMMA);

	free_tokens(tokens, 9);
}

void test_mixed_2() {

	printt("test mixed 2");

	char* str = ") f)~> ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == RPARENS);

	assert(tokens[1]->kind == ID);
	assert(strcmp(tokens[1]->value_ptr, "f") == 0);

	assert(tokens[2]->kind == RPARENS);

	assert(tokens[3]->kind == ARROW_SIDE_EFFECT);

	free_tokens(tokens, 4);
}

void test_mixed_3() {

	printt("test mixed 3");

	char* str = "if(x ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == IF);
	assert(tokens[1]->kind == LPARENS);
	assert(tokens[2]->kind == ID);

	free_tokens(tokens, 3);
}

void test_mixed_4() {

	printt("test mixed 4");

	char* str = "while(x uint n ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == WHILE);
	assert(tokens[1]->kind == LPARENS);
	assert(tokens[2]->kind == ID);
	assert(tokens[3]->kind == TYPEID_PRIMITIVE_UINT);
	assert(tokens[4]->kind == ID);

	free_tokens(tokens, 5);
}

void test_mixed_5() {

	printt("test mixed 5");

	char* str = "]='d'; ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == RBRACKET);
	assert(tokens[1]->kind == ASSIGNOP_SIMPLE);
	assert(tokens[2]->kind == CCONST);
	assert(tokens[3]->kind == SEMICOLON);

	free_tokens(tokens, 4);
}

void test_mixed_6() {

	printt("test mixed 6");

	char* str = "MyStruct s} ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == TYPEID);
	assert(tokens[1]->kind == ID);
	assert(tokens[2]->kind == RCURLY);

	free_tokens(tokens, 3);
}

void test_mixed_7() {

	printt("test mixed 7");

	char* str = "(1<2) ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == LPARENS);
	assert(tokens[1]->kind == INTEGER);
	assert(tokens[2]->kind == OPKEY_RELATIONAL_LT);
	assert(tokens[3]->kind == INTEGER);
	assert(tokens[4]->kind == RPARENS);

	free_tokens(tokens, 5);
}

void test_mixed_8() {

	printt("test mixed 8");

	char* str = "putchar('1') ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == ID);
	assert(tokens[1]->kind == LPARENS);
	assert(tokens[2]->kind == CCONST);
	assert(tokens[3]->kind == RPARENS);

	free_tokens(tokens, 4);
}

void test_mixed_9() {

	printt("test mixed 9");

	char* str = "putchar('-') ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == ID);
	assert(tokens[1]->kind == LPARENS);
	assert(tokens[2]->kind == CCONST);
	assert(tokens[3]->kind == RPARENS);

	free_tokens(tokens, 4);
}

void test_mixed_10() {

	printt("test mixed 10");

	char* str = ";\n 	i=0; ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == SEMICOLON);

	assert(tokens[1]->kind == ID);
	assert(tokens[1]->line_num == 2);

	assert(tokens[2]->kind == ASSIGNOP_SIMPLE);
	assert(tokens[3]->kind == INTEGER);

	free_tokens(tokens, 5);
}

void test_mixed_11() {

	printt("test mixed 11");

	char* str = "uint i, char j ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == TYPEID_PRIMITIVE_UINT);
	assert(tokens[1]->kind == ID);
	assert(tokens[2]->kind == COMMA);
	assert(tokens[3]->kind == TYPEID_PRIMITIVE_CHAR);
	assert(tokens[4]->kind == ID);

	free_tokens(tokens, 5);
}

void test_mixed_12() {

	printt("test mixed 12");

	char* str = "( x < 31) ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == LPARENS);
	assert(tokens[1]->kind == ID);
	assert(tokens[2]->kind == OPKEY_RELATIONAL_LT);
	assert(tokens[3]->kind == INTEGER);
	assert(tokens[4]->kind == RPARENS);

	free_tokens(tokens, 5);
}

void test_mixed_13() {

	printt("test mixed 13");

	char* str = "println(\"vector sum:\"); ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == ID);
	assert(tokens[1]->kind == LPARENS);

	assert(tokens[2]->kind == STRINGCONST);

	assert(tokens[3]->kind == RPARENS);
	assert(tokens[4]->kind == SEMICOLON);

	free_tokens(tokens, 5);
}

void test_mixed_14() {

	printt("test mixed 14");

	char* str = "struct MyStruct{ int a, int b}";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == STRUCT);
	assert(tokens[1]->kind == TYPEID);

	assert(tokens[2]->kind == LCURLY);

	assert(tokens[3]->kind == TYPEID_PRIMITIVE_INT);
	assert(tokens[4]->kind == ID);

	assert(tokens[5]->kind == COMMA);

	assert(tokens[6]->kind == TYPEID_PRIMITIVE_INT);
	assert(tokens[7]->kind == ID);

	assert(tokens[8]->kind == RCURLY);

	free_tokens(tokens, 9);
}

void test_mixed_15() {

	printt("test mixed 15");

	char* str = "struct MyStruct{\n\tint a,\n\tint b\n}";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == STRUCT);
	assert(tokens[0]->line_num == 1);

	assert(tokens[1]->kind == TYPEID);

	assert(tokens[2]->kind == LCURLY);

	assert(tokens[3]->kind == TYPEID_PRIMITIVE_INT);
	assert(tokens[4]->kind == ID);

	assert(tokens[5]->kind == COMMA);

	assert(tokens[6]->kind == TYPEID_PRIMITIVE_INT);
	assert(tokens[7]->kind == ID);

	assert(tokens[8]->kind == RCURLY);
	assert(tokens[8]->line_num == 5);

	free_tokens(tokens, 9);
}

void test_mixed_16() {

	printt("test mixed 16");

	char* str = "((uint) -> bool) member ";
	size_t count = 0;
	struct Token** tokens = lex(str, &count);

	assert(tokens[0]->kind == LPARENS);
	assert(tokens[1]->kind == LPARENS);
	assert(tokens[2]->kind == TYPEID_PRIMITIVE_UINT);

	assert(tokens[3]->kind == RPARENS);
	assert(tokens[4]->kind == ARROW_NO_SIDE_EFFECT);
	assert(tokens[5]->kind == TYPEID_PRIMITIVE_BOOL);
	assert(tokens[6]->kind == RPARENS);
	assert(tokens[7]->kind == ID);

	free_tokens(tokens, 8);
}
