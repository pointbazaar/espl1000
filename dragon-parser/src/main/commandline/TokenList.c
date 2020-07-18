#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "TokenList.h"
#include "TokenKeys.h"
#include "../../../../util/util.h"

struct TokenList* makeTokenList() {
	
	//this is enough for most tests
	const int initial_size = 20;

	//DEBUG
	//printf("makeTokenList()\n");

	struct TokenList* res = smalloc(sizeof(struct TokenList));
	
	strcpy(res->relPath, "/dev/null");
	res->tokensc = 0;

	res->tokens = smalloc(sizeof(struct Token*)*initial_size);
	res->indexHead = 0;

	res->capacity = initial_size;

	return res;
}

void list_add(struct TokenList* list, struct Token* token) {

	//DEBUG
	//printf("list_add\n");

	if((list->indexHead + list->tokensc + 1) > list->capacity){
		printf("resize TokenList instance\n");
		//we don't have enough capacity
		//double the capacity		
		list->capacity = list->capacity * 2;
		list->tokens = realloc(list->tokens, list->capacity * sizeof(struct Token*));
	}

	list->tokens[list->indexHead + list->tokensc] = token;
	list->tokensc += 1;
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
		return tokenEquals(
			list_head(list),
			token
		);
	}
	return false;
}

char* wrap(char* s, char* wrap) {
	char* res = smalloc(sizeof(char)*strlen(s)+2*strlen(wrap));
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

	char str[1000];
	str[0]='\0';
	strcat(str,"Syntax Error in ");
	strcat(str,list->relPath);
	strcat(str,":");

	char buf[20];
	sprintf(buf, "%d", list_head(list)->lineNumber);
	strcat(str, buf);
	strcat(str, ": expected: ");
	
	strcat(str,token->value);
	
	strcat(str," (");
	sprintf(buf, "%d", token->kind);
	strcat(str, buf);
	strcat(str,")");

	strcat(str,"\t actual: ");
	strcat(str,list_head(list)->value);
	
	strcat(str, " (");

	sprintf(buf, "%d", list_head(list)->kind);
	strcat(str, buf);

	strcat(str,")");
	strcat(str,"     ");
	strcat(str,list_code(list, false));
	
	strcat(str,"\n");
	
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

	struct TokenList* res = makeTokenList();

	strcpy(res->relPath, other->relPath);
	
	for(int i = 0; i < list_size(other); i++){
		
		list_add(res, list_get(other, i));
	}
	
	return res;
}

void list_set(struct TokenList* list, struct TokenList* copy) {
	
	free(list->tokens);
	
	list->tokens = smalloc(sizeof(struct Token*) * copy->capacity);

	list->capacity = copy->capacity;
	list->tokensc = copy->tokensc;
	list->indexHead = copy->indexHead;

	size_t size = copy->capacity * sizeof(struct Token*);
	memcpy(list->tokens, copy->tokens, size);
}

struct Token* list_get(struct TokenList* list, int i) {
	
	//this is to give an indication to what went wrong
	if( i >= list_size(list)){
		printf(
			"access at index %d, but list is only %d long\n",
			i,
			list_size(list)
		);
	}
	
	return list->tokens[list->indexHead + i];
}

struct Token* list_head(struct TokenList* list) {
	return list_get(list, 0);
}

char* list_code(struct TokenList* list, bool debug) {
	//it should be a limited fragment 

	if(debug){
		printf("list_code(...)\n");
	}

	char* str = smalloc(sizeof(char)*100);
	strcpy(str, "");

	
	for(int i=0;i < list_size(list) && (i < 10);i++){
		struct Token* tk = list_get(list,i);

		strcat(str, tk->value);
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
	size_t size = list->indexHead + list->tokensc;
	for(int i=0;i < size; i++){
		freeToken(list->tokens[i]);
	}
	freeTokenListShallow(list);
}

void freeTokenListShallow(struct TokenList* list){
	free(list->tokens);
	free(list);
}
