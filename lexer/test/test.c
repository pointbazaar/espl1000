/*
this file is supposed to test dragon-lexer
the testing shall be conducted via unit tests.
there is not much going on with the files so
we probably do not need integration tests here.
*/

//Stdlib Headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

//Project Headers
#include "test.h"
#include "../../token/token/token.h"
#include "../../token/TokenKeys.h"
#include "lexer_test_utils.h"

static int lexer_test_all();
static uint32_t test_num = 1;

static void printt(char* test_name){
	printf("[Lexer][Test][#%02d] %s", test_num++, test_name);
}

int main(){
	
	lexer_test_all();
}

int lexer_test_all() {

	printf("[Lexer] Running Tests... \n");

	test_try_catch_throw_throws();

	test_plus_plus_minus_minus();

	test_can_see_line_with_semicolon();
	test_can_see_line_with_operators();
	test_lexes_return_statement_favorably();
	test_lexes_other_return_statement();
	test_lexes_float_constant();
	test_lexes_escaped_char();

	test_anytypetoken();

	test_true();
	test_false();

	test_char();

	test_float_1();
	test_float_2();
	test_float_3();

	test_identifier_1();
	test_identifier_2();

	test_int_1();
	test_int_2();
	test_int_3();
	test_hex();
	test_binconst();

	test_struct();
	test_return();

	test_string_1();

	test_typeidentifier();
	test_typeidentifier_primitive();

	test_typeparameter_1();

	test_comma();
	test_arrow();
	test_mixed_1();
	test_mixed_2();
	test_mixed_3();
	test_mixed_4();
	test_mixed_5();
	test_mixed_6();
	test_mixed_7();
	test_mixed_8();
	test_mixed_9();
	test_mixed_10();
	test_mixed_11();
	test_mixed_12();
	test_mixed_13();
	test_mixed_14();
	test_mixed_15();
	test_mixed_16();

	test_operators();
	test_operators_cmp();
	test_operators_logical();
	test_operators_bitwise();

	test_if_else();

	test_break();
	test_for();
	test_in();

	test_rangeop_1();
	test_rangeop_2();

	test_member_access();

	test_switch_case();

	test_assign_operators();
	test_brackets();

	printf("[Lexer] All %i Tests Passed\n", test_num-1);

	return 0;
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

	//free_tokens(tokens, 4); //segfaults
}

void test_plus_plus_minus_minus(){
	
	char* str = "Char ++ -- ";
	//should expand: "Char += 1 -= 1"
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == TYPEID);
	assert(tokens[1]->kind == ASSIGNOP);
	assert(tokens[2]->kind == INTEGER);
	assert(tokens[3]->kind == ASSIGNOP);
	assert(tokens[4]->kind == INTEGER);

	assert(strcmp(tokens[1]->value_ptr,"+=")==0);
	assert(strcmp(tokens[2]->value_ptr,"1")==0);
	assert(strcmp(tokens[3]->value_ptr,"-=")==0);
	assert(strcmp(tokens[4]->value_ptr,"1")==0);

	free_tokens(tokens, 5);
}

bool test_can_see_line_with_semicolon() {

	printt("test can see line with semicolon\n");

	char* str = "Char x; ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == TYPEID);
	assert(tokens[1]->kind == ID);
	assert(tokens[2]->kind == SEMICOLON);

	assert(strcmp(tokens[1]->value_ptr,"x")==0);

	free_tokens(tokens, 3);

	return true;
}

bool test_can_see_line_with_operators() {

	printt("can see line with operators\n");

	char* str = "x = x+x; ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == ID);
	assert(tokens[1]->kind == ASSIGNOP);
	assert(tokens[2]->kind == ID);
	assert(tokens[3]->kind == OPKEY_ARITHMETIC);
	assert(tokens[4]->kind == ID);

	free_tokens(tokens, 5);

	return true;
}

bool test_lexes_return_statement_favorably() {

	printt("lexes return statement favorably\n");

	char* str = "return (-5)*n; ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==RETURN);

	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==OPKEY_ARITHMETIC);
	assert(tokens[3]->kind==INTEGER);
	assert(tokens[4]->kind==RPARENS);

	assert(tokens[5]->kind==OPKEY_ARITHMETIC);
	assert(tokens[6]->kind==ID);
	assert(tokens[7]->kind==SEMICOLON);

	free_tokens(tokens, 8);

	return true;
}
bool test_lexes_other_return_statement() {

	printt("lexes other return statement\n");

	char* str = "return (n*faculty(n-1)); ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==RETURN);

	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==ID);
	assert(tokens[3]->kind==OPKEY_ARITHMETIC);
	assert(tokens[4]->kind==ID);

	assert(tokens[5]->kind==LPARENS);
	assert(tokens[6]->kind==ID);
	assert(tokens[7]->kind==OPKEY_ARITHMETIC);
	assert(tokens[8]->kind==INTEGER);
	assert(tokens[9]->kind==RPARENS);
	assert(tokens[10]->kind==RPARENS);

	free_tokens(tokens, 11);

	return true;
}
bool test_lexes_float_constant() {

	printt("lexes float constant\n");

	char* str = "1.44 ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==FLOATING);
	assert(strcmp(tokens[0]->value_ptr,"1.44")==0);

	free_tokens(tokens, 1);

	return true;
}
bool test_lexes_escaped_char() {

	printt("lexes escaped char\n");

	char* str = "return '\\n'";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==RETURN);
	assert(tokens[1]->kind==CCONST);
	assert(strcmp(tokens[1]->value_ptr,"'\\n'")==0);

	free_tokens(tokens, 2);

	return true;
}


bool test_anytypetoken() {

	printt("test anytype token\n");

	char* str = "# ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==ANYTYPE);

	free_tokens(tokens, 1);

	return true;
}

bool test_true() {

	printt("test boolconst token\n");

	char* str = "true ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==BCONST_TRUE);

	free_tokens(tokens, 1);

	return true;
}

bool test_false() {

	printt("test boolconst token\n");

	char* str = "false ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==BCONST_FALSE);

	free_tokens(tokens, 1);

	return true;
}

bool test_char() {

	printt("test charconst token\n");

	char* str = "'x' ('\\n') 'h' ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==CCONST);
	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==CCONST);
	assert(tokens[3]->kind==RPARENS);

	assert(strcmp(tokens[0]->value_ptr,"'x'")==0 );

	assert(strcmp(tokens[2]->value_ptr,"'\\n'")==0 );
	
	assert(tokens[4]->kind == CCONST);
	assert(strcmp(tokens[4]->value_ptr,"'h'")==0 );

	free_tokens(tokens, 5);

	return true;
}


bool test_float_1() {

	printt("test floatconst token:1\n");

	char* str = "2038.4 ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==FLOATING);
	assert(strcmp(tokens[0]->value_ptr,"2038.4") == 0);

	free_tokens(tokens, 1);

	return true;
}

bool test_float_2() {

	printt("test floatconst token:2\n");

	char* str = "0.0 ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind == FLOATING);
	assert(strcmp(tokens[0]->value_ptr, "0.0") == 0);

	free_tokens(tokens, 1);

	return true;
}

bool test_float_3() {

	printt("test floatconst token:3\n");

	char* str = "-5.0 ";
	struct Token** tokens = lex(str);

	//looking at index 1 because '-' and float
	//are lexed seperately, for simplicity reasons.
	//perhaps we may change it later on

	assert(tokens[1]->kind==FLOATING);
	assert(strcmp(tokens[1]->value_ptr,"5.0") == 0);

	free_tokens(tokens, 2);

	return true;
}

bool test_identifier_1() {

	printt("test identifier token:1\n");

	char* str = "main ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==ID);
	assert(strcmp(tokens[0]->value_ptr,"main")==0);

	free_tokens(tokens, 1);

	return true;
}

bool test_identifier_2() {

	printt("test identifier token:2\n");

	char* str = "arg_ls ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==ID);
	assert(strcmp(tokens[0]->value_ptr,"arg_ls")==0);

	free_tokens(tokens, 1);

	return true;
}

bool test_int_1() {

	printt("test integer token:1\n");

	char* str = "2038 ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==INTEGER);
	assert(strcmp(tokens[0]->value_ptr,"2038")==0);

	free_tokens(tokens, 1);

	return true;
}

bool test_int_2() {

	printt("test integer token:2\n");

	char* str = "0 ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==INTEGER);
	assert(strcmp(tokens[0]->value_ptr,"0")==0);

	free_tokens(tokens, 1);

	return true;
}

bool test_int_3() {

	printt("test integer token:3\n");

	char* str = "-5 ";
	struct Token** tokens = lex(str);

	assert(tokens[1]->kind==INTEGER);
	assert(strcmp(tokens[1]->value_ptr,"5")==0);

	free_tokens(tokens, 2);

	return true;
}

bool test_hex(){

	printt("test hex\n");

	char* str = "0x1 0x0 0x10 0x1f 0xA3";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==HEXCONST);
	assert(strcmp(tokens[0]->value_ptr,"0x1")==0);
	
	assert(tokens[1]->kind==HEXCONST);
	assert(strcmp(tokens[1]->value_ptr,"0x0")==0);
	
	assert(tokens[2]->kind==HEXCONST);
	assert(strcmp(tokens[2]->value_ptr,"0x10")==0);
	
	assert(tokens[3]->kind==HEXCONST);
	assert(strcmp(tokens[3]->value_ptr,"0x1f")==0);
	
	assert(tokens[4]->kind==HEXCONST);
	assert(strcmp(tokens[4]->value_ptr,"0xA3")==0);

	free_tokens(tokens, 5);

	return true;
}

bool test_binconst(){

	printt("test binconst\n");

	char* str = "0b0 0b1 0b10 0b101";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==BINCONST);
	assert(strcmp(tokens[0]->value_ptr,"0b0")==0);
	
	assert(tokens[1]->kind==BINCONST);
	assert(strcmp(tokens[1]->value_ptr,"0b1")==0);
	
	assert(tokens[2]->kind==BINCONST);
	assert(strcmp(tokens[2]->value_ptr,"0b10")==0);
	
	assert(tokens[3]->kind==BINCONST);
	assert(strcmp(tokens[3]->value_ptr,"0b101")==0);

	free_tokens(tokens, 4);

	return true;
}

bool test_struct() {

	printt("test struct token\n");

	char* str = "struct ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==STRUCT);

	free_tokens(tokens, 1);

	return true;
}

bool test_return() {

	printt("test return token\n");

	char* str = "return ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==RETURN);

	free_tokens(tokens, 1);

	return true;
}

bool test_string_1() {

	printt("test string token:1\n");

	char* str = "\"hi\" ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==STRINGCONST);
	
	assert(strcmp(tokens[0]->value_ptr,"hi")==0);

	free_tokens(tokens, 1);

	return true;
}


bool test_typeidentifier() {

	printt("test typeidentifier token:1\n");

	char* str = "Carrot ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==TYPEID);
	assert(strcmp(tokens[0]->value_ptr,"Carrot")==0);

	free_tokens(tokens, 1);

	return true;
}

bool test_typeidentifier_primitive(){

	printt("test typeidentifier primitive\n");

	char* str = "int uint int8 uint8";
	
	struct Token** tokens = lex(str);

	char* expect[] = {"int", "uint", "int8", "uint8"};

	for(int i=0;i < 4; i++){
		assert(tokens[i]->kind == TYPEID_PRIMITIVE_INT);
		assert(strcmp(tokens[i]->value_ptr, expect[i]) == 0);
	}

	free_tokens(tokens, 4);
	
	//------------------------------
	char* str2 = "float char bool ";
	
	struct Token** tokens2 = lex(str2);
	
	assert(tokens2[0]->kind == TYPEID_PRIMITIVE_FLOAT);
	assert(tokens2[1]->kind == TYPEID_PRIMITIVE_CHAR);
	assert(tokens2[2]->kind == TYPEID_PRIMITIVE_BOOL);

	free_tokens(tokens2, 3);

	return true;
}

bool test_typeparameter_1() {

	printt("test typeparameter token\n");

	char* str = "?T0 ?T1 (?T2";
	struct Token** tokens = lex(str);
	
	assert(tokens[0]->kind==TPARAM);
	assert(tokens[1]->kind==TPARAM);
	assert(tokens[2]->kind==LPARENS);
	assert(tokens[3]->kind==TPARAM);
	assert(strcmp(tokens[0]->value_ptr,"0")==0);
	free_tokens(tokens, 4);
	return true;
}

bool test_comma() {

	printt("test comma token\n");

	char* str = ",, ";
	struct Token** tokens = lex(str);

	assert(tokens[1]->kind==COMMA);
	assert(strcmp(tokens[1]->value_ptr,",")==0);

	free_tokens(tokens, 2);

	return true;
}

bool test_arrow() {

	printt("test arrow token\n");

	char* str = "-> ~> ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==ARROW);
	assert(strcmp(tokens[0]->value_ptr,"->")==0);

	assert(tokens[1]->kind==ARROW);
	assert(strcmp(tokens[1]->value_ptr,"~>")==0);

	free_tokens(tokens, 2);

	return true;
}

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

bool test_operators() {

	printt("test operators\n");

	char* str = "+ - * / % ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==OPKEY_ARITHMETIC);
	assert(strcmp(tokens[0]->value_ptr, "+") == 0);
	
	assert(tokens[1]->kind==OPKEY_ARITHMETIC);
	assert(strcmp(tokens[1]->value_ptr, "-") == 0);
	
	assert(tokens[2]->kind==OPKEY_ARITHMETIC);
	assert(strcmp(tokens[2]->value_ptr, "*") == 0);
	
	assert(tokens[3]->kind==OPKEY_ARITHMETIC);
	assert(strcmp(tokens[3]->value_ptr, "/") == 0);
	
	assert(tokens[4]->kind==OPKEY_ARITHMETIC);
	assert(strcmp(tokens[4]->value_ptr, "%") == 0);

	free_tokens(tokens, 5);

	return true;
}

bool test_operators_cmp(){
	
	printt("test operators_cmp\n");

	char* str = "<= >= == != < >";
	struct Token** tokens = lex(str);
	
	assert(tokens[0]->kind==OPKEY_RELATIONAL);
	assert(strcmp(tokens[0]->value_ptr, "<=") == 0);
	
	assert(tokens[1]->kind==OPKEY_RELATIONAL);
	assert(strcmp(tokens[1]->value_ptr, ">=") == 0);
	
	assert(tokens[2]->kind==OPKEY_RELATIONAL);
	assert(strcmp(tokens[2]->value_ptr, "==") == 0);
	
	assert(tokens[3]->kind==OPKEY_RELATIONAL);
	assert(strcmp(tokens[3]->value_ptr, "!=") == 0);
	
	assert(tokens[4]->kind==OPKEY_RELATIONAL);
	assert(strcmp(tokens[4]->value_ptr, "<") == 0);
	
	assert(tokens[5]->kind==OPKEY_RELATIONAL);
	assert(strcmp(tokens[5]->value_ptr, ">") == 0);

	free_tokens(tokens, 6);

	return true;
}

bool test_operators_logical(){
	
	printt("test operators_logical\n");

	char* str = "&& || ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==OPKEY_LOGICAL);
	assert(strcmp(tokens[0]->value_ptr, "&&") == 0);
	
	assert(tokens[1]->kind==OPKEY_LOGICAL);
	assert(strcmp(tokens[1]->value_ptr, "||") == 0);

	free_tokens(tokens, 2);

	return true;
}

bool test_operators_bitwise(){
	
	printt("test operators_bitwise\n");

	char* str = "| & << >> ^ ~";
	struct Token** tokens = lex(str);
	
	assert(tokens[0]->kind==OPKEY_BITWISE);
	assert(strcmp(tokens[0]->value_ptr, "|") == 0);
	
	assert(tokens[1]->kind==OPKEY_BITWISE);
	assert(strcmp(tokens[1]->value_ptr, "&") == 0);
	
	assert(tokens[2]->kind==OPKEY_BITWISE);
	assert(strcmp(tokens[2]->value_ptr, "<<") == 0);
	
	assert(tokens[3]->kind==OPKEY_BITWISE);
	assert(strcmp(tokens[3]->value_ptr, ">>") == 0);
	
	assert(tokens[4]->kind==OPKEY_BITWISE);
	assert(strcmp(tokens[4]->value_ptr, "^") == 0);
	
	assert(tokens[5]->kind==OPKEY_BITWISE);
	assert(strcmp(tokens[5]->value_ptr, "~") == 0);

	free_tokens(tokens, 6);

	return true;
}

bool test_if_else() {

	printt("test if else\n");

	char* str = "if else{ ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==IF);
	assert(tokens[1]->kind==ELSE);
	assert(tokens[2]->kind==LCURLY);

	free_tokens(tokens, 3);

	return true;
}

bool test_break() {

	printt("test break\n");

	char* str = "break; int x";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==BREAK);
	assert(tokens[1]->kind==SEMICOLON);
	assert(tokens[2]->kind==TYPEID_PRIMITIVE_INT);

	free_tokens(tokens, 3);

	return true;
}

bool test_for(){

	printt("test_for\n");

	char* str = "for for{ ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==FOR);
	assert(tokens[1]->kind==FOR);
	assert(tokens[2]->kind==LCURLY);

	free_tokens(tokens, 3);

	return true;
}

bool test_in(){

	printt("test_in\n");

	char* str = "in; in{ ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==IN);
	assert(tokens[1]->kind==SEMICOLON);
	assert(tokens[2]->kind==IN);
	assert(tokens[3]->kind==LCURLY);

	free_tokens(tokens, 4);

	return true;
}

bool test_rangeop_1(){

	printt("test_rangeop_1\n");

	char* str = ".. a.... ";
	struct Token** tokens = lex(str);
	
	assert(tokens[0]->kind==RANGEOP);
	assert(tokens[1]->kind==ID);
	assert(tokens[2]->kind==RANGEOP);
	assert(tokens[3]->kind==RANGEOP);

	free_tokens(tokens, 4);

	return true;
}

bool test_rangeop_2(){

	printt("test_rangeop_1\n");

	char* str = "0 .. 3 a .. b ";
	struct Token** tokens = lex(str);

	assert(tokens[0]->kind==INTEGER);
	assert(tokens[1]->kind==RANGEOP);
	assert(tokens[2]->kind==INTEGER);
	
	assert(tokens[3]->kind==ID);
	assert(tokens[4]->kind==RANGEOP);
	assert(tokens[5]->kind==ID);

	free_tokens(tokens, 6);

	return true;
}

bool test_member_access(){

	printt("test_member_access\n");

	char* str = "s.member= ";
	struct Token** tokens = lex(str);
	
	assert(tokens[0]->kind==ID);
	assert(tokens[1]->kind==STRUCTMEMBERACCESS);
	assert(tokens[2]->kind==ID);
	
	assert(tokens[3]->kind==ASSIGNOP);

	free_tokens(tokens, 4);

	return true;
}

bool test_switch_case(){

	printt("test_switch_case\n");

	char* str = "switch x { case 3  ";
	struct Token** tokens = lex(str);
	
	assert(tokens[0]->kind==SWITCH);
	assert(tokens[1]->kind==ID);
	assert(tokens[2]->kind==LCURLY);
	
	assert(tokens[3]->kind==CASE);
	assert(tokens[4]->kind==INTEGER);

	free_tokens(tokens, 5);

	return true;
}

bool test_assign_operators(){

	printt("test_assign_operators\n");

	char* str = "= += -= *= /=  >>= <<= &= |= %=";
	struct Token** tokens = lex(str);
	
	// =
	assert(tokens[0]->kind==ASSIGNOP);
	assert(strcmp(tokens[0]->value_ptr, "=") == 0);
	
	// arithmetic
	assert(tokens[1]->kind==ASSIGNOP);
	assert(strcmp(tokens[1]->value_ptr, "+=") == 0);
	
	assert(tokens[2]->kind==ASSIGNOP);
	assert(strcmp(tokens[2]->value_ptr, "-=") == 0);
	
	assert(tokens[3]->kind==ASSIGNOP);
	assert(strcmp(tokens[3]->value_ptr, "*=") == 0);
	
	assert(tokens[4]->kind==ASSIGNOP);
	assert(strcmp(tokens[4]->value_ptr, "/=") == 0);
	
	// bitwise
	assert(tokens[5]->kind==ASSIGNOP);
	assert(strcmp(tokens[5]->value_ptr, ">>=") == 0);
	
	assert(tokens[6]->kind==ASSIGNOP);
	assert(strcmp(tokens[6]->value_ptr, "<<=") == 0);
	
	assert(tokens[7]->kind==ASSIGNOP);
	assert(strcmp(tokens[7]->value_ptr, "&=") == 0);
	
	assert(tokens[8]->kind==ASSIGNOP);
	assert(strcmp(tokens[8]->value_ptr, "|=") == 0);
	
	//modulo equals
	assert(tokens[9]->kind==ASSIGNOP);
	assert(strcmp(tokens[9]->value_ptr, "%=") == 0);

	free_tokens(tokens, 10);
	return true;
}

bool test_brackets(){

	printt("test_brackets\n");

	char* str = "[](){}< > ";
	struct Token** tokens = lex(str);
	
	assert(tokens[0]->kind==LBRACKET);
	assert(tokens[1]->kind==RBRACKET);
	assert(tokens[2]->kind==LPARENS);
	assert(tokens[3]->kind==RPARENS);
	assert(tokens[4]->kind==LCURLY);
	assert(tokens[5]->kind==RCURLY);
	assert(tokens[6]->kind==OPKEY_RELATIONAL);
	assert(tokens[7]->kind==OPKEY_RELATIONAL);
	
	assert( strcmp(tokens[6]->value_ptr,"<")==0 );
	assert( strcmp(tokens[7]->value_ptr,">")==0 );

	free_tokens(tokens, 8);
	return true;
}
