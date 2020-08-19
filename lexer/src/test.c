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

//Project Headers
#include "tokens.h"
#include "lexer.h"
#include "loop.h"
#include "test.h"

bool assert(bool condition){
	//to make it clear to the reader
	//that this is the assertion of the test

	if(condition){
		printf("assertion passed\n");
	}else{
		printf("assertion failed\n");
		exit(1);
	}

	return condition;
}

bool debug;

int test_all(bool debug1){
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
	count+=3;

	pass+=test_struct();
	pass+=test_return();
	count+=2;

	pass+=test_string_1();
	pass+=test_string_2();
	count+=2;

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
	pass+=test_mixed_13();
	count+=15;

	pass+=test_operators();
	count+=1;

	pass+=test_loop();
	pass+=test_if_else();
	count+=2;

	printf("%i of %i tests passed\n",pass,count);

	return 0;
}


bool test_can_see_line_with_semicolon(){
	if(debug){
		printf("test can see line with semicolon\n");
	}
	//this test is to see if the lexer can see
	//an entire line

	char* str = "Char x; ";
	struct Token** tokens = lex(str,".test.tokens");
	
	bool res = true;
	
	res &= assert(tokens[0]->kind == TYPEIDENTIFIER);
	res &= assert(tokens[1]->kind == IDENTIFIER);
	res &= assert(tokens[2]->kind == SEMICOLON);

	res &= assert(strcmp(tokens[1]->value,"x")==0);

	return res;
}

bool test_can_see_line_with_operators(){
	if(debug){
		printf("test can see line with operators\n");
	}
	//this test is to see if the lexer can see
	//an entire line

	char* str = "x = x+x; ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind == IDENTIFIER);
	res &= assert(tokens[1]->kind == EQ);
	res &= assert(tokens[2]->kind == IDENTIFIER);
	res &= assert(tokens[3]->kind == OPERATOR);
	res &= assert(tokens[4]->kind == IDENTIFIER);

	return res;
}

bool test_lexes_return_statement_favorably(){
	if(debug){
		printf("test lexes return statement favorably\n");
	}

	char* str = "return (-5)*n; ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==RETURN);

	res &= assert(tokens[1]->kind==LPARENS);
	res &= assert(tokens[2]->kind==OPERATOR);
	res &= assert(tokens[3]->kind==INTEGER);
	res &= assert(tokens[4]->kind==RPARENS);

	res &= assert(tokens[5]->kind==OPERATOR);
	res &= assert(tokens[6]->kind==IDENTIFIER);
	res &= assert(tokens[7]->kind==SEMICOLON);

	return res;
}
bool test_lexes_other_return_statement(){
	if(debug){
		printf("test lexes other return statement\n");
	}
	
	char* str = "return (n*faculty(n-1)); ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==RETURN);
	
	res &= assert(tokens[1]->kind==LPARENS);
	res &= assert(tokens[2]->kind==IDENTIFIER);
	res &= assert(tokens[3]->kind==OPERATOR);
	res &= assert(tokens[4]->kind==IDENTIFIER);

	res &= assert(tokens[5]->kind==LPARENS);
	res &= assert(tokens[6]->kind==IDENTIFIER);
	res &= assert(tokens[7]->kind==OPERATOR);
	res &= assert(tokens[8]->kind==INTEGER);
	res &= assert(tokens[9]->kind==RPARENS);
	res &= assert(tokens[10]->kind==RPARENS);

	return res;
}
bool test_lexes_float_constant(){
	if(debug){
		printf("test lexes float constant\n");
	}
	
	char* str = "1.44 ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==FLOATING);
	res &= assert(strcmp(tokens[0]->value,"1.44")==0);

	return res;
}
bool test_lexes_escaped_char(){
	if(debug){
		printf("test lexes escaped char\n");
	}
	
	char* str = "return '\\n'";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==RETURN);
	res &= assert(tokens[1]->kind==CHARCONST);
	res &= assert(strcmp(tokens[1]->value,"'\\n'")==0);

	return res;
}


bool test_anytypetoken(){
	if(debug){
		printf("test anytype token\n");
	}
	
	char* str = "# ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==ANYTYPE);

	return res;
}

bool test_true(){
	if(debug){
		printf("test boolconst token\n");
	}
	
	char* str = "true ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==BOOLCONST);

	return res;
}

bool test_false(){
	if(debug){
		printf("test boolconst token\n");
	}
	
	char* str = "false ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==BOOLCONST);

	return res;
}

bool test_char(){
	if(debug){
		printf("test charconst token\n");
	}
	
	char* str = "'x' ('\\n') ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==CHARCONST);
	res &= assert(tokens[1]->kind==LPARENS);
	res &= assert(tokens[2]->kind==CHARCONST);
	res &= assert(tokens[3]->kind==RPARENS);

	res &= assert(strcmp(tokens[0]->value,"'x'")==0 );

	res &= assert(strcmp(tokens[2]->value,"'\\n'")==0 );
	return res;

}


bool test_float_1(){
	if(debug){
		printf("test floatconst token:1\n");
	}
	
	char* str = "2038.4 ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==FLOATING);
	res &= assert(
		strcmp(tokens[0]->value,"2038.4")==0
	);
	return res;
}

bool test_float_2(){
	if(debug){
		printf("test floatconst token:2\n");
	}
	
	char* str = "0.0 ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==FLOATING);
	res &= assert(
		strcmp(tokens[0]->value,"0.0")==0
	);
	return res;
}

bool test_float_3(){
	if(debug){
		printf("test floatconst token:3\n");
	}
	
	char* str = "-5.0 ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	//looking at index 1 because '-' and float
	//are lexed seperately, for simplicity reasons.
	//perhaps we may change it later on 

	res &= assert(tokens[1]->kind==FLOATING);
	res &= assert(
		strcmp(tokens[1]->value,"5.0")==0
	);
	return res;
}

bool test_identifier_1(){
	if(debug){
		printf("test identifier token:1\n");
	}
	
	char* str = "main ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==IDENTIFIER);
	res &= assert(
		strcmp(tokens[0]->value,"main")==0
	);
	return res;
}

bool test_identifier_2(){
	if(debug){
		printf("test identifier token:2\n");
	}
	
	char* str = "arg_ls ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==IDENTIFIER);
	res &= assert(
		strcmp(tokens[0]->value,"arg_ls")==0
	);
	return res;
}

bool test_int_1(){
	if(debug){
		printf("test integer token:1\n");
	}
	
	char* str = "2038 ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==INTEGER);
	res &= assert(
		strcmp(tokens[0]->value,"2038")==0
	);
	return res;
}

bool test_int_2(){
	if(debug){
		printf("test integer token:2\n");
	}
	
	char* str = "0 ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==INTEGER);
	res &= assert(
		strcmp(tokens[0]->value,"0")==0
	);
	return res;
}

bool test_int_3(){
	if(debug){
		printf("test integer token:3\n");
	}
	
	char* str = "-5 ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[1]->kind==INTEGER);
	res &= assert(
		strcmp(tokens[1]->value,"5")==0
	);
	return res;
}

bool test_struct(){
	if(debug){
		printf("test struct token\n");
	}
	
	char* str = "struct ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==STRUCT);
	return res;
}

bool test_return(){
	if(debug){
		printf("test return token\n");
	}
	
	char* str = "return ";
	struct Token** tokens = lex(str,".test.tokens");

	bool res = true;

	res &= assert(tokens[0]->kind==RETURN);
	return res;
}

bool test_string_1(){
	if(debug){
		printf("test string token:1\n");
	}
	
	char* str = "\"hi\" ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==STRINGCONST);
	//printf("%s\n",get(tokens,0)->value);
	res &= assert(
		strcmp(tokens[0]->value,"\"hi\"")==0
	);
	return res;
}

bool test_string_2(){
	if(debug){
		printf("test string token:2\n");
	}
	
	char* str = "\"hi\n\nhi\" ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==STRINGCONST);
	res &= assert(
		strcmp(tokens[0]->value,"\"hi\n\nhi\"")==0
	);
	return res;
}


bool test_typeidentifier_simple(){
	if(debug){
		printf("test typeidentifier token:1\n");
	}
	
	char* str = "PInt ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==TYPEIDENTIFIER);
	res &= assert(
		strcmp(tokens[0]->value,"PInt")==0
	);
	return res;
}

bool test_typeidentifier_other(){
	if(debug){
		printf("test typeidentifier token:2\n");
	}
	
	char* str = "Point ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==TYPEIDENTIFIER);
	res &= assert(
		strcmp(tokens[0]->value,"Point")==0
	);
	return res;
}

bool test_typeidentifier_token(){
	if(debug){
		printf("test typeidentifier token:3\n");
	}
	
	char* str = "Char ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==TYPEIDENTIFIER);
	res &= assert(
		strcmp(tokens[0]->value,"Char")==0
	);
	return res;
}

bool test_typeparameter_1(){
	if(debug){
		printf("test typeparameter token\n");
	}
	
	char* str = "?T0 ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==TYPEPARAM);
	res &= assert(
		strcmp(tokens[0]->value,"0")==0
	);
	return res;
}

bool test_comma(){
	if(debug){
		printf("test comma token\n");
	}
	
	char* str = ",, ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[1]->kind==COMMA);
	res &= assert(
		strcmp(tokens[1]->value,",")==0
	);
	return res;
}

bool test_arrow(){
	if(debug){
		printf("test arrow token\n");
	}
	
	char* str = "-> ~> ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==ARROW);
	res &= assert(
		strcmp(tokens[0]->value,"->")==0
	);

	res &= assert(tokens[1]->kind==ARROW);
	res &= assert(
		strcmp(tokens[1]->value,"~>")==0
	);

	return res;
}

bool test_mixed_1(){

	if(debug){
		printf("test mixed 1\n");
	}
	
	char* str = "struct MyStruct{PInt a,MyStruct b} ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[2]->kind==LCURLY);

	res &= assert(tokens[3]->kind==TYPEIDENTIFIER);
	res &= assert(
		strcmp(tokens[3]->value,"PInt")==0
	);

	res &= assert(tokens[4]->kind==IDENTIFIER);
	res &= assert(
		strcmp(tokens[4]->value,"a")==0
	);

	res &= assert(tokens[5]->kind==COMMA);

	return res;
}

bool test_mixed_2(){

	if(debug){
		printf("test mixed 2\n");
	}
	
	char* str = ") f)~> ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==RPARENS);

	res &= assert(tokens[1]->kind==IDENTIFIER);
	res &= assert(
		strcmp(tokens[1]->value,"f")==0
	);

	res &= assert(tokens[2]->kind==RPARENS);

	res &= assert(tokens[3]->kind==ARROW);

	return res;
}

bool test_mixed_3(){

	if(debug){
		printf("test mixed 3\n");
	}
	
	char* str = "if(x ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==IF);

	res &= assert(tokens[1]->kind==LPARENS);

	res &= assert(tokens[2]->kind==IDENTIFIER);

	return res;
}

bool test_mixed_4(){

	if(debug){
		printf("test mixed 4\n");
	}
	
	char* str = "while(x PInt n ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==WHILE);

	res &= assert(tokens[1]->kind==LPARENS);

	res &= assert(tokens[2]->kind==IDENTIFIER);

	//PInt
	res &= assert(tokens[3]->kind==TYPEIDENTIFIER);

	//id
	res &= assert(tokens[4]->kind==IDENTIFIER);

	return res;
}

bool test_mixed_5(){

	if(debug){
		printf("test mixed 5\n");
	}

	//arr[1]='d';

	char* str = "]='d'; ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==RBRACKET);

	res &= assert(tokens[1]->kind==EQ);

	res &= assert(tokens[2]->kind==CHARCONST);

	res &= assert(tokens[3]->kind==SEMICOLON);

	return res;
}

bool test_mixed_6(){
	//MyStruct s}

	if(debug){
		printf("test mixed 6\n");
	}

	char* str = "MyStruct s} ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==TYPEIDENTIFIER);

	res &= assert(tokens[1]->kind==IDENTIFIER);

	res &= assert(tokens[2]->kind==RCURLY);

	return res;
}

bool test_mixed_7(){
	//(1<2)

	if(debug){
		printf("test mixed 7\n");
	}

	char* str = "(1<2) ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==LPARENS);

	res &= assert(tokens[1]->kind==INTEGER);

	res &= assert(tokens[2]->kind==LESSER);

	res &= assert(tokens[3]->kind==INTEGER);

	res &= assert(tokens[4]->kind==RPARENS);

	return res;
}

bool test_mixed_8(){
	//putchar('1')

	if(debug){
		printf("test mixed 8\n");
	}

	char* str = "putchar('1') ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==IDENTIFIER);

	res &= assert(tokens[1]->kind==LPARENS);

	res &= assert(tokens[2]->kind==CHARCONST);

	res &= assert(tokens[3]->kind==RPARENS);

	return res;
}

bool test_mixed_9(){
	//putchar('-');

	if(debug){
		printf("test mixed 9\n");
	}

	char* str = "putchar('-') ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==IDENTIFIER);

	res &= assert(tokens[1]->kind==LPARENS);

	res &= assert(tokens[2]->kind==CHARCONST);

	res &= assert(tokens[3]->kind==RPARENS);

	return res;
}

bool test_mixed_10(){

	//;
    //    i=0;

	if(debug){
		printf("test mixed 10\n");
	}

	char* str = ";\n 	i=0; ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==SEMICOLON);

	res &= assert(tokens[1]->kind==LINE_NO);

	res &= assert(tokens[2]->kind==IDENTIFIER);

	res &= assert(tokens[3]->kind==EQ);

	res &= assert(tokens[4]->kind==INTEGER);

	return res;
}

bool test_mixed_11(){

	//PInt i, PInt j

	if(debug){
		printf("test mixed 11\n");
	}

	char* str = "PInt i, PInt j ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==TYPEIDENTIFIER);

	res &= assert(tokens[1]->kind==IDENTIFIER);

	res &= assert(tokens[2]->kind==COMMA);

	res &= assert(tokens[3]->kind==TYPEIDENTIFIER);

	res &= assert(tokens[4]->kind==IDENTIFIER);

	return res;
}

bool test_mixed_12(){
	// x < 3.1

	if(debug){
		printf("test mixed 12\n");
	}

	char* str = "( x < 3.1) ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==LPARENS);

	res &= assert(tokens[1]->kind==IDENTIFIER);

	res &= assert(tokens[2]->kind==LESSER);

	res &= assert(tokens[3]->kind==FLOATING);

	res &= assert(tokens[4]->kind==RPARENS);

	return res;
}

bool test_mixed_13(){

	//println("vector sum:");

	if(debug){
		printf("test mixed 13\n");
	}

	char* str = "println(\"vector sum:\"); ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==IDENTIFIER);
	res &= assert(tokens[1]->kind==LPARENS);

	res &= assert(tokens[2]->kind==STRINGCONST);

	res &= assert(tokens[3]->kind==RPARENS);
	res &= assert(tokens[4]->kind==SEMICOLON);

	return res;
}

bool test_operators(){

	if(debug){
		printf("test operators\n");
	}
	
	char* str = "+ - * / && || ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==OPERATOR);

	res &= assert(tokens[1]->kind==OPERATOR);

	res &= assert(tokens[2]->kind==OPERATOR);

	res &= assert(tokens[3]->kind==OPERATOR);

	res &= assert(tokens[4]->kind==OPERATOR);

	res &= assert(tokens[5]->kind==OPERATOR);

	return res;
}

bool test_loop(){

	if(debug){
		printf("test loop\n");
	}
	
	char* str = "loop loop{ lo ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==LOOP);

	res &= assert(tokens[1]->kind==LOOP);

	res &= assert(tokens[2]->kind==LCURLY);

	res &= assert(tokens[3]->kind==IDENTIFIER);

	return res;
}

bool test_if_else(){

	if(debug){
		printf("test if else\n");
	}
	
	char* str = "if else{ ";
	struct Token** tokens = lex(str,".test.tokens");
	bool res = true;

	res &= assert(tokens[0]->kind==IF);

	res &= assert(tokens[1]->kind==ELSE);

	res &= assert(tokens[2]->kind==LCURLY);

	//res &= assert(get(tokens,3)->kind==IDENTIFIER);

	return res;
}
