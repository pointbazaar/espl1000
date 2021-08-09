#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../test.h"
#include "../../token/token/token.h"
#include "../../token/TokenKeys.h"
#include "../lexer_test_utils.h"

void test_if_else() {

	printt("test if else\n");

	char* str = "if else{ ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==IF);
	assert(tokens[1]->kind==ELSE);
	assert(tokens[2]->kind==LCURLY);

	free_tokens(tokens, 3);
}

void test_break() {

	printt("test break\n");

	char* str = "break; int x";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==BREAK);
	assert(tokens[1]->kind==SEMICOLON);
	assert(tokens[2]->kind==TYPEID_PRIMITIVE_INT);

	free_tokens(tokens, 3);
}

void test_for(){

	printt("test_for\n");

	char* str = "for for{ ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==FOR);
	assert(tokens[1]->kind==FOR);
	assert(tokens[2]->kind==LCURLY);

	free_tokens(tokens, 3);
}

void test_in(){

	printt("test_in\n");

	char* str = "in; in{ ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==IN);
	assert(tokens[1]->kind==SEMICOLON);
	assert(tokens[2]->kind==IN);
	assert(tokens[3]->kind==LCURLY);

	free_tokens(tokens, 4);
}

void test_switch_case(){

	printt("test_switch_case\n");

	char* str = "switch x { case 3  ";
	struct Token** tokens = lex(str);
	
	assert(tokens[0]->kind==SWITCH);
	assert(tokens[1]->kind==ID);
	assert(tokens[2]->kind==LCURLY);
	
	assert(tokens[3]->kind==CASE);
	assert(tokens[4]->kind==INTEGER);

	free_tokens(tokens, 5);
}

void test_try_catch_throw_throws(){
	
	char* str = "try catch throw throws ";
	
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == TRY);
	assert(tokens[1]->kind == CATCH);
	assert(tokens[2]->kind == THROW);
	assert(tokens[3]->kind == THROWS);

	assert(strcmp(tokens[0]->value_ptr,"try")==0);
	assert(strcmp(tokens[1]->value_ptr,"catch")==0);
	assert(strcmp(tokens[2]->value_ptr,"throw")==0);
	assert(strcmp(tokens[3]->value_ptr,"throws")==0);

	free_tokens(tokens, 4);
}
