#include "TokenListTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"

#include <stdio.h>
#include <stdbool.h>

int test_tokenlist1(bool debug){

	if(debug){
		printf("test_tokenlist1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));
	list_add(list, makeToken2(OPKEY,"="));
	list_add(list, makeToken2(INTEGER,"4"));

	bool assert1 = list_size(list) == 3;

	list_add(list, makeToken(SEMICOLON));

	bool assert2 = list_size(list) == 4;

	return (assert1 && assert2)?1:0;

}

int test_tokenlist_consume(bool debug){

	if(debug){
		printf("test_tokenlist_consume\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));
	
	bool assert1 = list_size(list) == 1;

	list_consume(list, 1);

	bool assert2 = list_size(list) == 0;

	return (assert1 && assert2)?1:0;
}

int test_tokenlist_get(bool debug){

	if(debug){
		printf("test_tokenlist_get\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));
	
	struct Token* tk = list_get(list, 0);

	bool assert1 = tk->kind == ID;
	bool assert2 = strcmp(tk->value, "x") == 0;

	return (assert1 && assert2)?1:0;
}

int test_tokenlist_startswith(bool debug){

	if(debug){
		printf("test_tokenlist_startswith\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));

	bool assert1 = list_startsWith(list, makeToken2(ID,"x"));

	return (assert1)?1:0;
}