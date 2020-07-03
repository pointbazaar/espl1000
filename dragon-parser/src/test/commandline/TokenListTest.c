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
	list_add(list, makeToken2(INTEGER,"0"));
	list_add(list, makeToken2(RBRACKET,"]"));
	list_add(list, makeToken2(ID,"z"));
	
	struct Token* tk = list_get(list, 0);
	bool a1 = tk->kind == ID;
	bool a2 = strcmp(tk->value, "x") == 0;

	struct Token* tk2 = list_get(list,1);
	bool a3 = tk2->kind == INTEGER;
	bool a4 = strcmp(tk2->value,"0") == 0;

	struct Token* tk3 = list_get(list,2);
	bool a5 = tk3->kind == RBRACKET;
	bool a6 = strcmp(tk3->value,"]") == 0;

	struct Token* tk4 = list_get(list,3);
	bool a7 = tk4->kind == ID;
	bool a8 = strcmp(tk4->value,"z") == 0;

	if(debug){
		for(int k=0;k < list_size(list) && (k < 10);k++){
			struct Token* tk = list_get(list,k);
		
			printf(" %d", tk->kind);
		}
		printf("\n");
	}

	return (a1 && a2 && a3 && a4 && a5 && a6 && a7 && a8)?1:0;
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

int test_tokenlist_code(bool debug){

	if(debug){
		printf("test_tokenlist_code\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x"));
	list_add(list, makeToken2(ID,"z"));
	list_add(list, makeToken2(ID,"y"));
	list_add(list, makeToken2(RBRACKET,"]"));

	for(int k=0;k < list_size(list) && (k < 10);k++){
		struct Token* tk = list_get(list,k);
		
		printf(" %d", tk->kind);
	}

	char* str = list_code(list, debug);
	char* expect = "x z y ]     [4,4,4,25,]";
	if(debug){
		printf("expected:\n");
		printf("%s\n", expect);
		printf("actual:\n");
		printf("%s\n", str);
	}
	bool assert1 = strcmp(str, expect) == 0;

	return (assert1)?1:0;
}