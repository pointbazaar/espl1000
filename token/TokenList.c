#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "TokenList.h"
#include "TokenKeys.h"
#include "token.h"

struct TokenList* makeTokenList() {
	
	//this is enough for most tests
	const int initial_size = 20;

	//DEBUG
	//printf("makeTokenList()\n");

	struct TokenList* res = malloc(sizeof(struct TokenList));
	
	strcpy(res->relPath, "/dev/null");
	res->tokensc = 0;

	res->tokens = malloc(sizeof(struct Token*)*initial_size);
	res->indexHead = 0;
	res->tokensStored = 0;

	res->capacity = initial_size;

	return res;
}

void list_add(struct TokenList* list, struct Token* token) {

	//DEBUG
	//printf("list_add\n");

	if((list->indexHead + list->tokensc + 1) > list->capacity){
		
		//DEBUG
		//printf("resize TokenList instance\n");
		
		//we don't have enough capacity
		//double the capacity		
		list->capacity = list->capacity * 2;
		list->tokens = realloc(list->tokens, list->capacity * sizeof(struct Token*));
	}
	
	
	list->tokens[list->indexHead + list->tokensc] = token;
	list->tokensc += 1;
	list->tokensStored += 1;
}

void list_consume(struct TokenList* list, int amount) {
	
	list->tokensc -= amount;
	list->indexHead += amount;
}

int list_size(struct TokenList* list) {
	return list->tokensc;
}

bool list_startsWith(struct TokenList* list, struct Token* token) {
	//we cannot rely on .equals()
	//we should use interfaces we can rely on classes to implement
	//the class and the content of the token should be the same for them to be the same

	if (list_size(list) > 0) {
		const bool res = tokenEquals(
			list_head(list),
			token
		);
		
		return res;
	}
	return false;
}

char* wrap(char* s, char* wrap) {
	char* res = malloc(sizeof(char)*strlen(s)+2*strlen(wrap));
	sprintf(res,"%s%s%s",wrap,s,wrap);
	return res;
}

bool list_expect_internal(struct TokenList* list, struct Token* token) {
	//it expect a token or a kind of token, otherwise throws an exception
	if (list_size(list) == 0) {
		return false;
	}
	if (list_startsWith(list, token)) {
		list_consume(list, 1);
		return true;
	}
	
	return false;
}

bool list_expect(struct TokenList* list, int token_kind){

	struct Token* tk = makeToken(token_kind);
	const bool res = list_expect_internal(list, tk);
	freeToken(tk);
	return res;
}

bool list_expect_2(struct TokenList* list, struct Token* tk){
	return list_expect_internal(list, tk);
}

struct TokenList* list_copy(struct TokenList* other) {

	//DEBUG
	//printf("list_copy(...)\n");

	struct TokenList* list = makeTokenList();
	list_set(list, other);
	
	return list;
}

void list_set(struct TokenList* list, struct TokenList* copy) {
	
	free(list->tokens);
	
	list->tokens = malloc(sizeof(struct Token*) * copy->capacity);

	list->capacity = copy->capacity;
	list->tokensc = copy->tokensc;
	list->indexHead = copy->indexHead;
	list->tokensStored = copy->tokensStored;

	size_t size = copy->capacity * sizeof(struct Token*);
	memcpy(list->tokens, copy->tokens, size);
}

struct Token* list_get(struct TokenList* list, int i) {
	
	//no bounds checking! for performance.
	
	return list->tokens[list->indexHead + i];
}

struct Token* list_head(struct TokenList* list) {
	return list_get(list, 0);
}

char* list_code(struct TokenList* list, bool debug) {
	//it should be a limited fragment 

	if(debug){ printf("list_code(...)\n"); }

	char* str = malloc(sizeof(char)*100);
	strcpy(str, "");

	if(list_size(list) > 0){
		int line_num = list_get(list, 0)->lineNumber;
		sprintf(str, "% 4d|", line_num);
	}
	
	for(int i=0;i < list_size(list) && (i < 10);i++){
		struct Token* tk = list_get(list,i);

		strcat(str, tk->value_ptr);
		strcat(str, " ");
	}
	strcat(str,"    ");
	strcat(str,"[");

	char buf[100];
	strcpy(buf,"");

	for(int k=0;k < list_size(list) && (k < 10);k++){
		struct Token* tk = list_get(list,k);
		
		sprintf(buf, "%d", tk->kind);

		strcat(str, buf);
		strcat(str,",");
	}
	strcat(str,"]");

	return str;
}

void list_print(struct TokenList* list){
	
	char* str = list_code(list, false);
	printf("%s\n", str);
	free(str);
}

void freeTokenList(struct TokenList* list){
	
	//also frees the tokens within,
	//even those already consumed
	
	for(int i=0;i < list->tokensStored; i++){
		//DEBUG
		//printf("free element %d\n", i);
		freeToken(list->tokens[i]);
	}
	freeTokenListShallow(list);
}

void freeTokenListShallow(struct TokenList* list){
	free(list->tokens);
	free(list);
}
