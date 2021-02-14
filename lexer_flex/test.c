/*
this file is supposed to test dragon-lexer

the testing shall be conducted via unit tests.

there is not much going on with the files so
we probably do not need integration tests here.

the tests shall be the same (or more) than there were
for the java version of this lexer
*/

//Stdlib Headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

//Project Headers
#include "driver.h"
#include "test.h"
#include "../token/token.h"

#include "../parser/main/commandline/TokenKeys.h"

struct Token** lex(char* source, bool debug){
	
	if(debug){
		printf("lex(%s, ...)\n", source);
	}

	//make a file with the source
	//run the lexer
	//read out the tokens
	
	char* fname_src = "/tmp/temporary-smalldragon.dg";
	char* fname_tks = "/tmp/.temporary-smalldragon.dg.tokens";
	
	//make a file with the source
	FILE* f = fopen(fname_src, "w");
	fprintf(f, "%s",source);
	fclose(f);
	
	
	//run the lexer
	char cmd[100];
	sprintf(cmd, "./dragon-lexer %s", fname_src);
	system(cmd);
	
	
	//read out the tokens
	FILE* ftks = fopen(fname_tks, "r");
	
	int count = 0;
	int capacity = 10;
	struct Token** tks = malloc(sizeof(struct Token*)*capacity);
	
	int id;
	char buffer[100];
	while(fscanf(ftks, "%d %s\n", &id, buffer) == 2){
		
		tks[count] = makeToken2(id, buffer);;
		count++;
		
		if(count >= capacity){
			capacity *= 2;
			tks = realloc(tks, sizeof(struct Token*)*capacity);
		}
	}
	fclose(ftks);
	
	return tks;
}

void freeTokens(struct Token** tokens, unsigned int count) {
	for(int i = 0; i < count; i++) {
		//free(tokens[i]->value);
		free(tokens[i]);
	}
	//we do not free 'tokens' as it is statically allocated
	//free(tokens);
}

bool debug;

int test_all(bool debug1) {
	printf("running tests \n");

	debug = debug1;

	int pass  = 0;
	int count = 0;

	pass+=test_can_see_line_with_semicolon();
	pass+=test_can_see_line_with_operators();
	pass+=test_lexes_return_statement_favorably();
	pass+=test_lexes_other_return_statement();
	pass+=test_lexes_float_constant();
	pass+=test_lexes_escaped_char();
	count+=6;

	pass+=test_anytypetoken();
	count+=1;

	pass+=test_true();
	pass+=test_false();
	count+=2;

	pass+=test_char();
	count+=1;

	pass+=test_float_1();
	pass+=test_float_2();
	pass+=test_float_3();
	count+=3;

	pass+=test_identifier_1();
	pass+=test_identifier_2();
	count+=2;

	pass+=test_int_1();
	pass+=test_int_2();
	pass+=test_int_3();
	pass+=test_hex();
	count+=4;

	pass+=test_struct();
	pass+=test_return();
	count+=2;

	pass+=test_string_1();
	count+=1;

	pass+=test_typeidentifier_simple();
	pass+= test_typeidentifier_other();
	pass+= test_typeidentifier_token();
	count+=3;

	pass+=test_typeparameter_1();
	count+=1;

	pass+=test_comma();
	pass+=test_arrow();
	pass+=test_mixed_1();
	pass+=test_mixed_2();
	pass+=test_mixed_3();
	pass+=test_mixed_4();
	pass+=test_mixed_5();
	pass+=test_mixed_6();
	pass+=test_mixed_7();
	pass+=test_mixed_8();
	pass+=test_mixed_9();
	pass+=test_mixed_10();
	pass+=test_mixed_11();
	pass+=test_mixed_12();
	//pass+=test_mixed_13(); //(it segfaults)
	pass+=test_mixed_14();
	pass+=test_mixed_15();
	pass+=test_mixed_16();
	count+=17;

	pass+=test_operators();
	pass+=test_operators_cmp();
	pass+=test_operators_logical();
	pass+=test_operators_bitwise();
	count+=4;

	pass+=test_loop();
	pass+=test_if_else();
	count+=2;

	pass+=test_break();
	pass+=test_for();
	pass+=test_in();
	count+=3;
	
	//these use assert
	pass+=test_rangeop_1();
	pass+=test_rangeop_2();
	count+=2;
	
	pass+=test_member_access();
	count+=1;
	
	pass+=test_switch_case();
	count+=1;
	
	pass+=test_assign_operators();
	pass+=test_brackets();
	count+=2;

	printf("%i of %i tests passed\n",pass,count);

	return (pass == count)?0:1;
}


bool test_can_see_line_with_semicolon() {

	if(debug) {
		printf("test can see line with semicolon\n");
	}

	char* str = "Char x; ";
	struct Token** tokens = lex(str, debug);

	assert(tokens[0]->kind == TYPEIDENTIFIER);
	assert(tokens[1]->kind == ID);
	assert(tokens[2]->kind == SEMICOLON);

	assert(strcmp(tokens[1]->value_ptr,"x")==0);

	freeTokens(tokens, 3);

	return true;
}

bool test_can_see_line_with_operators() {

	if(debug) {
		printf("test can see line with operators\n");
	}

	char* str = "x = x+x; ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind == ID);
	assert(tokens[1]->kind == ASSIGNOP);
	assert(tokens[2]->kind == ID);
	assert(tokens[3]->kind == OPKEY);
	assert(tokens[4]->kind == ID);

	freeTokens(tokens, 5);

	return true;
}

bool test_lexes_return_statement_favorably() {

	if(debug) {
		printf("test lexes return statement favorably\n");
	}

	char* str = "return (-5)*n; ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==RETURN);

	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==OPKEY);
	assert(tokens[3]->kind==INTEGER);
	assert(tokens[4]->kind==RPARENS);

	assert(tokens[5]->kind==OPKEY);
	assert(tokens[6]->kind==ID);
	assert(tokens[7]->kind==SEMICOLON);

	freeTokens(tokens, 8);

	return true;
}
bool test_lexes_other_return_statement() {

	if(debug) {
		printf("test lexes other return statement\n");
	}

	char* str = "return (n*faculty(n-1)); ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==RETURN);

	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==ID);
	assert(tokens[3]->kind==OPKEY);
	assert(tokens[4]->kind==ID);

	assert(tokens[5]->kind==LPARENS);
	assert(tokens[6]->kind==ID);
	assert(tokens[7]->kind==OPKEY);
	assert(tokens[8]->kind==INTEGER);
	assert(tokens[9]->kind==RPARENS);
	assert(tokens[10]->kind==RPARENS);

	freeTokens(tokens, 11);

	return true;
}
bool test_lexes_float_constant() {

	if(debug) {
		printf("test lexes float constant\n");
	}

	char* str = "1.44 ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==FLOATING);
	assert(strcmp(tokens[0]->value_ptr,"1.44")==0);

	freeTokens(tokens, 1);

	return true;
}
bool test_lexes_escaped_char() {

	if(debug) {
		printf("test lexes escaped char\n");
	}

	char* str = "return '\\n'";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==RETURN);
	assert(tokens[1]->kind==CCONST);
	assert(strcmp(tokens[1]->value_ptr,"'\\n'")==0);

	freeTokens(tokens, 2);

	return true;
}


bool test_anytypetoken() {

	if(debug) {
		printf("test anytype token\n");
	}

	char* str = "# ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==ANYTYPE);

	freeTokens(tokens, 1);

	return true;
}

bool test_true() {

	if(debug) {
		printf("test boolconst token\n");
	}

	char* str = "true ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==BCONST_TRUE);

	freeTokens(tokens, 1);

	return true;
}

bool test_false() {

	if(debug) {
		printf("test boolconst token\n");
	}

	char* str = "false ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==BCONST_FALSE);

	freeTokens(tokens, 1);

	return true;
}

bool test_char() {

	if(debug) {
		printf("test charconst token\n");
	}

	char* str = "'x' ('\\n') ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==CCONST);
	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==CCONST);
	assert(tokens[3]->kind==RPARENS);

	assert(strcmp(tokens[0]->value_ptr,"'x'")==0 );

	assert(strcmp(tokens[2]->value_ptr,"'\\n'")==0 );

	freeTokens(tokens, 4);

	return true;
}


bool test_float_1() {

	if(debug) {
		printf("test floatconst token:1\n");
	}

	char* str = "2038.4 ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==FLOATING);
	assert(
	    strcmp(tokens[0]->value_ptr,"2038.4")==0
	);

	freeTokens(tokens, 1);

	return true;
}

bool test_float_2() {
	if(debug) {
		printf("test floatconst token:2\n");
	}

	char* str = "0.0 ";
	struct Token** tokens = 
		lex(str, debug);



	assert(tokens[0]->kind==FLOATING);
	assert(
	    strcmp(tokens[0]->value_ptr,"0.0")==0
	);

	freeTokens(tokens, 1);

	return true;
}

bool test_float_3() {
	if(debug) {
		printf("test floatconst token:3\n");
	}

	char* str = "-5.0 ";
	struct Token** tokens = 
		lex(str, debug);

	//looking at index 1 because '-' and float
	//are lexed seperately, for simplicity reasons.
	//perhaps we may change it later on

	assert(tokens[1]->kind==FLOATING);
	assert(
	    strcmp(tokens[1]->value_ptr,"5.0")==0
	);

	freeTokens(tokens, 2);

	return true;
}

bool test_identifier_1() {
	if(debug) {
		printf("test identifier token:1\n");
	}

	char* str = "main ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==ID);
	assert(
	    strcmp(tokens[0]->value_ptr,"main")==0
	);

	freeTokens(tokens, 1);

	return true;
}

bool test_identifier_2() {
	if(debug) {
		printf("test identifier token:2\n");
	}

	char* str = "arg_ls ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==ID);
	assert(
	    strcmp(tokens[0]->value_ptr,"arg_ls")==0
	);

	freeTokens(tokens, 1);

	return true;
}

bool test_int_1() {
	if(debug) {
		printf("test integer token:1\n");
	}

	char* str = "2038 ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==INTEGER);
	assert(
	    strcmp(tokens[0]->value_ptr,"2038")==0
	);

	freeTokens(tokens, 1);

	return true;
}

bool test_int_2() {
	if(debug) {
		printf("test integer token:2\n");
	}

	char* str = "0 ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==INTEGER);
	assert(
	    strcmp(tokens[0]->value_ptr,"0")==0
	);

	freeTokens(tokens, 1);

	return true;
}

bool test_int_3() {
	if(debug) {
		printf("test integer token:3\n");
	}

	char* str = "-5 ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[1]->kind==INTEGER);
	assert(
	    strcmp(tokens[1]->value_ptr,"5")==0
	);

	freeTokens(tokens, 2);

	return true;
}

bool test_hex(){
	
	if(debug) {
		printf("test hex\n");
	}

	char* str = "0x1 0x0 0x10 ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==HEXCONST);
	assert(strcmp(tokens[0]->value_ptr,"0x1")==0);
	
	assert(tokens[1]->kind==HEXCONST);
	assert(strcmp(tokens[1]->value_ptr,"0x0")==0);
	
	assert(tokens[2]->kind==HEXCONST);
	assert(strcmp(tokens[2]->value_ptr,"0x10")==0);

	freeTokens(tokens, 3);

	return true;
}

bool test_struct() {
	if(debug) {
		printf("test struct token\n");
	}

	char* str = "struct ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==STRUCT);

	freeTokens(tokens, 1);

	return true;
}

bool test_return() {
	if(debug) {
		printf("test return token\n");
	}

	char* str = "return ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==RETURN);

	freeTokens(tokens, 1);

	return true;
}

bool test_string_1() {
	if(debug) {
		printf("test string token:1\n");
	}

	char* str = "\"hi\" ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==STRINGCONST);
	
	assert(
	    strcmp(tokens[0]->value_ptr,"\"hi\"")==0
	);

	freeTokens(tokens, 1);

	return true;
}


bool test_typeidentifier_simple() {
	if(debug) {
		printf("test typeidentifier token:1\n");
	}

	char* str = "PInt ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==TYPEIDENTIFIER);
	assert(
	    strcmp(tokens[0]->value_ptr,"PInt")==0
	);

	freeTokens(tokens, 1);

	return true;
}

bool test_typeidentifier_other() {
	if(debug) {
		printf("test typeidentifier token:2\n");
	}

	char* str = "Point ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==TYPEIDENTIFIER);
	assert(
	    strcmp(tokens[0]->value_ptr,"Point")==0
	);

	freeTokens(tokens, 1);

	return true;
}

bool test_typeidentifier_token() {
	if(debug) {
		printf("test typeidentifier token:3\n");
	}

	char* str = "Char ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==TYPEIDENTIFIER);
	assert(
	    strcmp(tokens[0]->value_ptr,"Char")==0
	);

	freeTokens(tokens, 1);

	return true;
}

bool test_typeparameter_1() {
	if(debug) {
		printf("test typeparameter token\n");
	}

	char* str = "?T0 ?T1 (?T2";
	struct Token** tokens = 
		lex(str, debug);
	
	assert(tokens[0]->kind==TPARAM);
	assert(tokens[1]->kind==TPARAM);
	assert(tokens[2]->kind==LPARENS);
	assert(tokens[3]->kind==TPARAM);
	assert(
	    strcmp(tokens[0]->value_ptr,"0")==0
	);
	freeTokens(tokens, 4);
	return true;
}

bool test_comma() {
	if(debug) {
		printf("test comma token\n");
	}

	char* str = ",, ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[1]->kind==COMMA);
	assert(
	    strcmp(tokens[1]->value_ptr,",")==0
	);

	freeTokens(tokens, 2);

	return true;
}

bool test_arrow() {
	if(debug) {
		printf("test arrow token\n");
	}

	char* str = "-> ~> ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==ARROW);
	assert(
	    strcmp(tokens[0]->value_ptr,"->")==0
	);

	assert(tokens[1]->kind==ARROW);
	assert(
	    strcmp(tokens[1]->value_ptr,"~>")==0
	);

	freeTokens(tokens, 2);

	return true;
}

bool test_mixed_1() {

	if(debug) {
		printf("test mixed 1\n");
	}

	char* str = "struct MyStruct{PInt a,MyStruct b} ";
	struct Token** tokens = 
		lex(str, debug);


	assert(tokens[2]->kind==LCURLY);

	assert(tokens[3]->kind==TYPEIDENTIFIER);
	assert(
	    strcmp(tokens[3]->value_ptr,"PInt")==0
	);

	assert(tokens[4]->kind==ID);
	assert(
	    strcmp(tokens[4]->value_ptr,"a")==0
	);

	assert(tokens[5]->kind==COMMA);

	freeTokens(tokens, 6);

	return true;
}

bool test_mixed_2() {

	if(debug) {
		printf("test mixed 2\n");
	}

	char* str = ") f)~> ";
	struct Token** tokens = 
		lex(str, debug);


	assert(tokens[0]->kind==RPARENS);

	assert(tokens[1]->kind==ID);
	assert(
	    strcmp(tokens[1]->value_ptr,"f")==0
	);

	assert(tokens[2]->kind==RPARENS);

	assert(tokens[3]->kind==ARROW);

	freeTokens(tokens, 4);

	return true;
}

bool test_mixed_3() {

	if(debug) {
		printf("test mixed 3\n");
	}

	char* str = "if(x ";
	struct Token** tokens = 
		lex(str, debug);


	assert(tokens[0]->kind==IF);

	assert(tokens[1]->kind==LPARENS);

	assert(tokens[2]->kind==ID);

	freeTokens(tokens, 3);

	return true;
}

bool test_mixed_4() {

	if(debug) {
		printf("test mixed 4\n");
	}

	char* str = "while(x PInt n ";
	struct Token** tokens = 
		lex(str, debug);


	assert(tokens[0]->kind==WHILE);

	assert(tokens[1]->kind==LPARENS);

	assert(tokens[2]->kind==ID);

	assert(tokens[3]->kind==TYPEIDENTIFIER);

	assert(tokens[4]->kind==ID);

	freeTokens(tokens, 5);

	return true;
}

bool test_mixed_5() {

	if(debug) {
		printf("test mixed 5\n");
	}
	
	char* str = "]='d'; ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==RBRACKET);

	assert(tokens[1]->kind==ASSIGNOP);

	assert(tokens[2]->kind==CCONST);

	assert(tokens[3]->kind==SEMICOLON);

	freeTokens(tokens, 4);

	return true;
}

bool test_mixed_6() {
	
	if(debug) {
		printf("test mixed 6\n");
	}

	char* str = "MyStruct s} ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==TYPEIDENTIFIER);

	assert(tokens[1]->kind==ID);

	assert(tokens[2]->kind==RCURLY);

	freeTokens(tokens, 3);

	return true;
}

bool test_mixed_7() {
	
	if(debug) {
		printf("test mixed 7\n");
	}

	char* str = "(1<2) ";
	struct Token** tokens = 
		lex(str, debug);


	assert(tokens[0]->kind==LPARENS);

	assert(tokens[1]->kind==INTEGER);

	assert(tokens[2]->kind==OPKEY);

	assert(tokens[3]->kind==INTEGER);

	assert(tokens[4]->kind==RPARENS);

	freeTokens(tokens, 5);

	return true;
}

bool test_mixed_8() {
	
	if(debug) {
		printf("test mixed 8\n");
	}

	char* str = "putchar('1') ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==ID);

	assert(tokens[1]->kind==LPARENS);

	assert(tokens[2]->kind==CCONST);

	assert(tokens[3]->kind==RPARENS);

	freeTokens(tokens, 4);

	return true;
}

bool test_mixed_9() {

	if(debug) {
		printf("test mixed 9\n");
	}

	char* str = "putchar('-') ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==ID);

	assert(tokens[1]->kind==LPARENS);

	assert(tokens[2]->kind==CCONST);

	assert(tokens[3]->kind==RPARENS);

	freeTokens(tokens, 4);

	return true;
}

bool test_mixed_10() {

	if(debug) {
		printf("test mixed 10\n");
	}

	char* str = ";\n 	i=0; ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==SEMICOLON);

	assert(tokens[1]->kind==LINE_NO);

	assert(tokens[2]->kind==ID);

	assert(tokens[3]->kind==ASSIGNOP);

	assert(tokens[4]->kind==INTEGER);

	freeTokens(tokens, 5);

	return true;
}

bool test_mixed_11() {

	if(debug) {
		printf("test mixed 11\n");
	}

	char* str = "PInt i, PInt j ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==TYPEIDENTIFIER);

	assert(tokens[1]->kind==ID);

	assert(tokens[2]->kind==COMMA);

	assert(tokens[3]->kind==TYPEIDENTIFIER);

	assert(tokens[4]->kind==ID);

	freeTokens(tokens, 5);

	return true;
}

bool test_mixed_12() {

	if(debug) {
		printf("test mixed 12\n");
	}

	char* str = "( x < 3.1) ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==LPARENS);

	assert(tokens[1]->kind==ID);

	assert(tokens[2]->kind==OPKEY);

	assert(tokens[3]->kind==FLOATING);

	assert(tokens[4]->kind==RPARENS);

	freeTokens(tokens, 5);

	return true;
}

bool test_mixed_13() {
	
	if(debug) {
		printf("test mixed 13\n");
	}

	char* str = "println(\"vector sum:\"); ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==ID);
	assert(tokens[1]->kind==LPARENS);

	assert(tokens[2]->kind==STRINGCONST);

	assert(tokens[3]->kind==RPARENS);
	assert(tokens[4]->kind==SEMICOLON);

	//TODO
	//freeTokens(tokens, 4);

	return true;
}

bool test_mixed_14(){
	
	if(debug) {
		printf("test mixed 14\n");
	}

	char* str = "struct MyStruct{ Int a, Int b}";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==STRUCT);
	assert(tokens[1]->kind==TYPEIDENTIFIER);

	assert(tokens[2]->kind==LCURLY);

	assert(tokens[3]->kind==TYPEIDENTIFIER);
	assert(tokens[4]->kind==ID);
	
	assert(tokens[5]->kind==COMMA);
	
	assert(tokens[6]->kind==TYPEIDENTIFIER);
	assert(tokens[7]->kind==ID);
	
	assert(tokens[8]->kind==RCURLY);

	freeTokens(tokens, 9);

	return true;
}

bool test_mixed_15(){
	
	if(debug) {
		printf("test mixed 15\n");
	}

	char* str = "struct MyStruct{\n\tInt a,\n\tInt b\n}";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==STRUCT);
	assert(tokens[1]->kind==TYPEIDENTIFIER);

	assert(tokens[2]->kind==LCURLY);
	//tokens[3] is LINE_NO
	assert(tokens[4]->kind==TYPEIDENTIFIER);
	assert(tokens[5]->kind==ID);
	
	assert(tokens[6]->kind==COMMA);
	//tokens[7] is LINE_NO
	assert(tokens[8]->kind==TYPEIDENTIFIER);
	assert(tokens[9]->kind==ID);
	//tokens[10] is LINE_NO
	assert(tokens[11]->kind==RCURLY);

	freeTokens(tokens, 12);

	return true;
}

bool test_mixed_16(){
	
	if(debug) { printf("test mixed 16\n"); }

	char* str = "((PInt) -> PInt) member ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==LPARENS);
	assert(tokens[1]->kind==LPARENS);
	assert(tokens[2]->kind==TYPEIDENTIFIER);
	
	assert(tokens[3]->kind==RPARENS);
	assert(tokens[4]->kind==ARROW);
	assert(tokens[5]->kind==TYPEIDENTIFIER);
	assert(tokens[6]->kind==RPARENS);
	assert(tokens[7]->kind==ID);

	freeTokens(tokens, 8);

	return true;
}

bool test_operators() {

	if(debug) {
		printf("test operators\n");
	}

	char* str = "+ - * / % ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==OPKEY);
	assert(strcmp(tokens[0]->value_ptr, "+") == 0);
	
	assert(tokens[1]->kind==OPKEY);
	assert(strcmp(tokens[1]->value_ptr, "-") == 0);
	
	assert(tokens[2]->kind==OPKEY);
	assert(strcmp(tokens[2]->value_ptr, "*") == 0);
	
	assert(tokens[3]->kind==OPKEY);
	assert(strcmp(tokens[3]->value_ptr, "/") == 0);
	
	assert(tokens[4]->kind==OPKEY);
	assert(strcmp(tokens[4]->value_ptr, "%") == 0);

	freeTokens(tokens, 5);

	return true;
}

bool test_operators_cmp(){
	
	if(debug) {
		printf("test operators_cmp\n");
	}

	char* str = "<= >= == != < >";
	struct Token** tokens = lex(str, debug);
	
	assert(tokens[0]->kind==OPKEY);
	assert(strcmp(tokens[0]->value_ptr, "<=") == 0);
	
	assert(tokens[1]->kind==OPKEY);
	assert(strcmp(tokens[1]->value_ptr, ">=") == 0);
	
	assert(tokens[2]->kind==OPKEY);
	assert(strcmp(tokens[2]->value_ptr, "==") == 0);
	
	assert(tokens[3]->kind==OPKEY);
	assert(strcmp(tokens[3]->value_ptr, "!=") == 0);
	
	assert(tokens[4]->kind==OPKEY);
	assert(strcmp(tokens[4]->value_ptr, "<") == 0);
	
	assert(tokens[5]->kind==OPKEY);
	assert(strcmp(tokens[5]->value_ptr, ">") == 0);

	freeTokens(tokens, 6);

	return true;
}

bool test_operators_logical(){
	
	if(debug) {
		printf("test operators_logical\n");
	}

	char* str = "&& || ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==OPKEY);
	assert(strcmp(tokens[0]->value_ptr, "&&") == 0);
	
	assert(tokens[1]->kind==OPKEY);
	assert(strcmp(tokens[1]->value_ptr, "||") == 0);

	freeTokens(tokens, 2);

	return true;
}

bool test_operators_bitwise(){
	
	if(debug) {
		printf("test operators_bitwise\n");
	}

	char* str = "| & << >> ^ ~";
	struct Token** tokens = 
		lex(str, debug);
	
	assert(tokens[0]->kind==OPKEY);
	assert(strcmp(tokens[0]->value_ptr, "|") == 0);
	
	assert(tokens[1]->kind==OPKEY);
	assert(strcmp(tokens[1]->value_ptr, "&") == 0);
	
	assert(tokens[2]->kind==OPKEY);
	assert(strcmp(tokens[2]->value_ptr, "<<") == 0);
	
	assert(tokens[3]->kind==OPKEY);
	assert(strcmp(tokens[3]->value_ptr, ">>") == 0);
	
	assert(tokens[4]->kind==OPKEY);
	assert(strcmp(tokens[4]->value_ptr, "^") == 0);
	
	assert(tokens[5]->kind==OPKEY);
	assert(strcmp(tokens[5]->value_ptr, "~") == 0);

	freeTokens(tokens, 6);

	return true;
}

bool test_loop() {

	if(debug) {
		printf("test loop\n");
	}

	char* str = "loop loop{ lo ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==LOOP);

	assert(tokens[1]->kind==LOOP);

	assert(tokens[2]->kind==LCURLY);

	assert(tokens[3]->kind==ID);

	freeTokens(tokens, 4);

	return true;
}

bool test_if_else() {

	if(debug) {
		printf("test if else\n");
	}

	char* str = "if else{ ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==IF);

	assert(tokens[1]->kind==ELSE);

	assert(tokens[2]->kind==LCURLY);

	freeTokens(tokens, 3);

	return true;
}

bool test_break() {

	if(debug) {
		printf("test break\n");
	}

	char* str = "break; Int x";
	struct Token** tokens = 
		lex(str, debug);


	assert(tokens[0]->kind==BREAK);

	assert(tokens[1]->kind==SEMICOLON);

	assert(tokens[2]->kind==TYPEIDENTIFIER);

	freeTokens(tokens, 3);

	return true;
}

bool test_for(){
	
	if(debug) {
		printf("test_for\n");
	}

	char* str = "for for{ ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==FOR);

	assert(tokens[1]->kind==FOR);

	assert(tokens[2]->kind==LCURLY);

	freeTokens(tokens, 3);

	return true;
}

bool test_in(){
	
	if(debug) {
		printf("test_in\n");
	}

	char* str = "in; in{ ";
	struct Token** tokens = 
		lex(str, debug);


	assert(tokens[0]->kind==IN);

	assert(tokens[1]->kind==SEMICOLON);

	assert(tokens[2]->kind==IN);
	
	assert(tokens[3]->kind==LCURLY);

	freeTokens(tokens, 4);

	return true;
}

bool test_rangeop_1(){
	
	if(debug) {
		printf("test_rangeop_1\n");
	}

	char* str = ".. a.... ";
	struct Token** tokens = 
		lex(str, debug);
	
	assert(tokens[0]->kind==RANGEOP);

	assert(tokens[1]->kind==ID);

	assert(tokens[2]->kind==RANGEOP);
	
	assert(tokens[3]->kind==RANGEOP);

	freeTokens(tokens, 4);

	return true;
}

bool test_rangeop_2(){
	
	if(debug) {
		printf("test_rangeop_1\n");
	}

	char* str = "0 .. 3 a .. b ";
	struct Token** tokens = 
		lex(str, debug);

	assert(tokens[0]->kind==INTEGER);
	assert(tokens[1]->kind==RANGEOP);
	assert(tokens[2]->kind==INTEGER);
	
	assert(tokens[3]->kind==ID);
	assert(tokens[4]->kind==RANGEOP);
	assert(tokens[5]->kind==ID);

	freeTokens(tokens, 6);

	return true;
}

bool test_member_access(){
	
	if(debug) { printf("test_member_access\n"); }

	char* str = "s.member= ";
	struct Token** tokens = 
		lex(str, debug);
	
	assert(tokens[0]->kind==ID);
	assert(tokens[1]->kind==STRUCTMEMBERACCESS);
	assert(tokens[2]->kind==ID);
	
	assert(tokens[3]->kind==ASSIGNOP);

	freeTokens(tokens, 4);

	return true;
}

bool test_switch_case(){
	
	if(debug) { printf("test_switch_case\n"); }

	char* str = "switch x { case 3  ";
	struct Token** tokens = 
		lex(str, debug);
	
	assert(tokens[0]->kind==SWITCH);
	assert(tokens[1]->kind==ID);
	assert(tokens[2]->kind==LCURLY);
	
	assert(tokens[3]->kind==CASE);
	assert(tokens[4]->kind==INTEGER);

	freeTokens(tokens, 5);

	return true;
}

bool test_assign_operators(){
	
	if(debug) { printf("test_assign_operators\n"); }

	char* str = "= += -= *= /=  ";
	struct Token** tokens = 
		lex(str, debug);
	
	assert(tokens[0]->kind==ASSIGNOP);
	
	assert(tokens[1]->kind==ASSIGNOP);
	assert(tokens[2]->kind==ASSIGNOP);
	assert(tokens[3]->kind==ASSIGNOP);
	assert(tokens[4]->kind==ASSIGNOP);

	freeTokens(tokens, 5);
	return true;
}

bool test_brackets(){
	
	if(debug) { printf("test_brackets\n"); }

	char* str = "[](){}< > ";
	struct Token** tokens = 
		lex(str, debug);
	
	assert(tokens[0]->kind==LBRACKET);
	assert(tokens[1]->kind==RBRACKET);
	assert(tokens[2]->kind==LPARENS);
	assert(tokens[3]->kind==RPARENS);
	assert(tokens[4]->kind==LCURLY);
	assert(tokens[5]->kind==RCURLY);
	assert(tokens[6]->kind==OPKEY);
	assert(tokens[7]->kind==OPKEY);
	
	assert( strcmp(tokens[6]->value_ptr,"<")==0 );
	assert( strcmp(tokens[7]->value_ptr,">")==0 );

	freeTokens(tokens, 8);
	return true;
}
