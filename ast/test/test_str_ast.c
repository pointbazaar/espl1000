#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "../util/free_ast.h"
#include "../util/str_ast.h"
#include "../ast.h"

static void status(char* msg){

	printf(" - [TEST] %s\n", msg);
}

//TODO: write more tests

void test_str_binconst(){
	
	status("strBinConst");
	
	struct BinConst* b = make(BinConst);
	
	b->value = 0b1010;
	
	char* s = strBinConst(b);
	
	assert(strcmp(s, "0b1010")==0);
	
	free(b); free(s);
}

void test_str_intconst(){
	
	status("strIntConst");
	
	struct IntConst* b = make(IntConst);
	
	b->value = 101;
	
	char* s = strIntConst(b);
	
	assert(strcmp(s, "101")==0);
	
	free(b); free(s);
}

void test_str_charconst(){
	
	status("strCharConst");
	
	struct CharConst* b = make(CharConst);
	
	b->value = 'h';
	
	char* s = strCharConst(b);
	
	assert(strcmp(s, "'h'")==0);
	
	free(b); free(s);
}

void test_str_floatconst(){
	
	status("strFloatConst");
	
	struct FloatConst* b = make(FloatConst);
	
	b->value = 1.483533;
	
	char* s = strFloatConst(b);
	
	assert(strcmp(s, "1.483533")==0);
	
	free(b); free(s);
}
