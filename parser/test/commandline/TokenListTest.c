#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>

#include "TokenListTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../../token/token.h"

int test_tokenlist1(bool debug){

	if(debug){
		printf("TEST: test_tokenlist1\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));
	list_add(list, makeToken2(OPKEY,"="));
	list_add(list, makeToken2(INTEGER,"4"));

	assert(list_size(list) == 3);

	list_add(list, makeToken(SEMICOLON));

	assert(list_size(list) == 4);
	
	freeTokenList(list);

	return 1;

}

int test_tokenlist_consume(bool debug){

	if(debug){
		printf("TEST: test_tokenlist_consume\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));
	
	assert(list_size(list) == 1);

	list_consume(list, 1);

	assert(list_size(list) == 0);
	
	//introspection
	assert(list->indexHead == 1);
	
	freeTokenList(list);

	return 1;
}

int test_tokenlist_get(bool debug){

	if(debug){
		printf("TEST: test_tokenlist_get\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));
	list_add(list, makeToken2(INTEGER,"0"));
	list_add(list, makeToken2(RBRACKET,"]"));
	list_add(list, makeToken2(ID,"z"));
	
	struct Token* tk = list_get(list, 0);
	bool a1 = tk->kind == ID;
	bool a2 = strcmp(tk->value_ptr, "x") == 0;

	struct Token* tk2 = list_get(list,1);
	bool a3 = tk2->kind == INTEGER;
	bool a4 = strcmp(tk2->value_ptr,"0") == 0;

	struct Token* tk3 = list_get(list,2);
	bool a5 = tk3->kind == RBRACKET;
	bool a6 = strcmp(tk3->value_ptr,"]") == 0;

	struct Token* tk4 = list_get(list,3);
	bool a7 = tk4->kind == ID;
	bool a8 = strcmp(tk4->value_ptr,"z") == 0;
	
	freeTokenList(list);

	return (a1 && a2 && a3 && a4 && a5 && a6 && a7 && a8)?1:0;
}

int test_tokenlist_startswith(bool debug){

	if(debug){
		printf("TEST: test_tokenlist_startswith\n");
	}

	struct TokenList* list = makeTokenList();

	struct Token* tk = makeToken2(ID,"x");
	list_add(list, tk);

	assert(list_startsWith(list, tk));
	
	freeTokenList(list);

	return 1;
}

int test_tokenlist_code(bool debug){

	if(debug){
		printf("TEST: test_tokenlist_code\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));
	list_add(list, makeToken2(ID,"z"));
	list_add(list, makeToken2(ID,"y"));
	list_add(list, makeToken2(RBRACKET,"]"));

	char* str = list_code(list, debug);
	char* expect = "x z y ]     [4,4,4,15,]";
	if(debug){
		printf("expected:\n");
		printf("%s\n", expect);
		printf("actual:\n");
		printf("%s\n", str);
	}
	
	assert(strcmp(str, expect) == 0);
	
	free(str);
	freeTokenList(list);

	return 1;
}

int test_tokenlist_stresstest(bool debug){
	//add and consume tokens
	//trigger a resize
	if(debug){
		printf("TEST: test_tokenlist_stresstest\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));
	list_add(list, makeToken2(ID,"x"));
	
	list_consume(list, 1);
	
	for(int i=0;i< 100;i++){
		list_add(list, makeToken2(ID,"x"));
	}
	
	struct Token* tk = list_head(list);
	bool a1 = tk != NULL;
	
	freeTokenList(list);

	return (a1)?1:0;
}
