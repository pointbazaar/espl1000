#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../test.h"
#include "../../token/token/token.h"
#include "../../token/TokenKeys.h"
#include "../lexer_test_utils.h"

bool test_mixed_1() {

	printt("test mixed 1\n");

	char* str = "struct MyStruct { uint a, MyStruct b} ";
	struct Token** tokens = lex(str);

	assert(tokens[2]->kind == LCURLY);

	assert(tokens[3]->kind == TYPEID_PRIMITIVE_INT);
	assert(strcmp(tokens[3]->value_ptr, "uint") == 0);

	assert(tokens[4]->kind == ID);
	assert(strcmp(tokens[4]->value_ptr, "a") == 0);

	assert(tokens[5]->kind == COMMA);

	free_tokens(tokens, 6);

	return true;
}

bool test_mixed_2() {

	printt("test mixed 2\n");

	char* str = ") f)~> ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==RPARENS);

	assert(tokens[1]->kind==ID);
	assert(strcmp(tokens[1]->value_ptr,"f")==0);

	assert(tokens[2]->kind==RPARENS);

	assert(tokens[3]->kind==ARROW);

	free_tokens(tokens, 4);

	return true;
}

bool test_mixed_3() {

	printt("test mixed 3\n");

	char* str = "if(x ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==IF);
	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==ID);

	free_tokens(tokens, 3);

	return true;
}

bool test_mixed_4() {

	printt("test mixed 4\n");

	char* str = "while(x uint n ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==WHILE);
	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==ID);
	assert(tokens[3]->kind==TYPEID_PRIMITIVE_INT);
	assert(tokens[4]->kind==ID);

	free_tokens(tokens, 5);

	return true;
}

bool test_mixed_5() {

	printt("test mixed 5\n");
	
	char* str = "]='d'; ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==RBRACKET);
	assert(tokens[1]->kind==ASSIGNOP);
	assert(tokens[2]->kind==CCONST);
	assert(tokens[3]->kind==SEMICOLON);

	free_tokens(tokens, 4);

	return true;
}

bool test_mixed_6() {

	printt("test mixed 6\n");

	char* str = "MyStruct s} ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==TYPEID);
	assert(tokens[1]->kind==ID);
	assert(tokens[2]->kind==RCURLY);

	free_tokens(tokens, 3);

	return true;
}

bool test_mixed_7() {

	printt("test mixed 7\n");

	char* str = "(1<2) ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==LPARENS);
	assert(tokens[1]->kind==INTEGER);
	assert(tokens[2]->kind==OPKEY_RELATIONAL);
	assert(tokens[3]->kind==INTEGER);
	assert(tokens[4]->kind==RPARENS);

	free_tokens(tokens, 5);

	return true;
}

bool test_mixed_8() {

	printt("test mixed 8\n");

	char* str = "putchar('1') ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==ID);
	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==CCONST);
	assert(tokens[3]->kind==RPARENS);

	free_tokens(tokens, 4);

	return true;
}

bool test_mixed_9() {

	printt("test mixed 9\n");

	char* str = "putchar('-') ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==ID);
	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==CCONST);
	assert(tokens[3]->kind==RPARENS);

	free_tokens(tokens, 4);

	return true;
}

bool test_mixed_10() {

	printt("test mixed 10\n");
	

	char* str = ";\n 	i=0; ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==SEMICOLON);

	assert(tokens[1]->kind==ID);
	assert(tokens[1]->line_num == 2);

	assert(tokens[2]->kind==ASSIGNOP);
	assert(tokens[3]->kind==INTEGER);

	free_tokens(tokens, 4);

	return true;
}

bool test_mixed_11() {

	printt("test mixed 11\n");

	char* str = "uint i, char j ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==TYPEID_PRIMITIVE_INT);
	assert(tokens[1]->kind==ID);
	assert(tokens[2]->kind==COMMA);
	assert(tokens[3]->kind==TYPEID_PRIMITIVE_CHAR);
	assert(tokens[4]->kind==ID);

	free_tokens(tokens, 5);

	return true;
}

bool test_mixed_12() {

	printt("test mixed 12\n");

	char* str = "( x < 3.1) ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==LPARENS);
	assert(tokens[1]->kind==ID);
	assert(tokens[2]->kind==OPKEY_RELATIONAL);
	assert(tokens[3]->kind==FLOATING);
	assert(tokens[4]->kind==RPARENS);

	free_tokens(tokens, 5);

	return true;
}

bool test_mixed_13() {
	
	printt("test mixed 13\n");

	char* str = "println(\"vector sum:\"); ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==ID);
	assert(tokens[1]->kind==LPARENS);

	assert(tokens[2]->kind==STRINGCONST);

	assert(tokens[3]->kind==RPARENS);
	assert(tokens[4]->kind==SEMICOLON);

	free_tokens(tokens, 5);

	return true;
}

bool test_mixed_14(){
	
	printt("test mixed 14\n");

	char* str = "struct MyStruct{ int a, float b}";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==STRUCT);
	assert(tokens[1]->kind==TYPEID);

	assert(tokens[2]->kind==LCURLY);

	assert(tokens[3]->kind==TYPEID_PRIMITIVE_INT);
	assert(tokens[4]->kind==ID);
	
	assert(tokens[5]->kind==COMMA);
	
	assert(tokens[6]->kind==TYPEID_PRIMITIVE_FLOAT);
	assert(tokens[7]->kind==ID);
	
	assert(tokens[8]->kind==RCURLY);

	free_tokens(tokens, 9);

	return true;
}

bool test_mixed_15(){
	
	printt("test mixed 15\n");

	char* str = "struct MyStruct{\n\tInt a,\n\tInt b\n}";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==STRUCT);
	assert(tokens[0]->line_num == 1);

	assert(tokens[1]->kind==TYPEID);

	assert(tokens[2]->kind==LCURLY);

	assert(tokens[3]->kind==TYPEID);
	assert(tokens[4]->kind==ID);
	
	assert(tokens[5]->kind==COMMA);

	assert(tokens[6]->kind==TYPEID);
	assert(tokens[7]->kind==ID);

	assert(tokens[8]->kind==RCURLY);
	assert(tokens[8]->line_num == 4);

	free_tokens(tokens, 9);

	return true;
}

bool test_mixed_16(){
	
	printt("test mixed 16\n"); 

	char* str = "((uint) -> bool) member ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==LPARENS);
	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==TYPEID_PRIMITIVE_INT);
	
	assert(tokens[3]->kind==RPARENS);
	assert(tokens[4]->kind==ARROW);
	assert(tokens[5]->kind==TYPEID_PRIMITIVE_BOOL);
	assert(tokens[6]->kind==RPARENS);
	assert(tokens[7]->kind==ID);

	free_tokens(tokens, 8);

	return true;
}
