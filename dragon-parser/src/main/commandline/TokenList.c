#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "TokenList.h"
#include "TokenKeys.h"

//this is to facilitate special features
//which would be convenient in a token list for our compiler

struct TokenList* makeTokenList_3(struct Token** result, int resultc, char* sourceFile) {

	struct TokenList* res = malloc(sizeof(struct TokenList));

	res->tokens = result;
	res->tokensc = resultc;
	res->capacity = resultc;

	res->relPath[0] = '\0';
	strcpy(res->relPath, sourceFile);

	return res;
}

struct TokenList* makeTokenList() {
	const int initial_size = 10;

	//DEBUG
	printf("makeTokenList()\n");

	struct TokenList* res = malloc(sizeof(struct TokenList));
	res->relPath[0]='\0';
	strcpy(res->relPath, "/dev/null");
	res->tokensc = 0;

	res->tokens = malloc(sizeof(struct Token*)*initial_size);
	res->capacity = initial_size;

	return res;
}

void list_add(struct TokenList* list, struct Token* token) {

	//DEBUG
	printf("list_add\n");

	if((list->tokensc + 1) > list->capacity){
		printf("resize TokenList instance\n");
		//we don't have enough capacity
		//double the capacity		
		list->capacity = list->capacity * 2;
		list->tokens = realloc(list->tokens, list->capacity * sizeof(struct Token*));
	}

	list->tokens[list->tokensc * sizeof(struct Token*)] = token;
	list->tokensc += 1;
}

void list_addAll(struct TokenList* list, struct Token** arr, int arrc) {
	for (int i = 0;i < arrc;i++) {
		
		list_add(list, arr[i]);
	}
}

void list_consume(struct TokenList* list, int amount) {
	for(int i = 0; i < amount; i++){
		list->tokens = list->tokens+(sizeof(struct Token*)*1);
		list->tokensc -= 1;
	}
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
			list_get(list,0),
			token
		);
	}
	return false;
}

bool list_endsWith(struct TokenList* list, struct Token* token) {
	if (list_size(list) > 0) {
		return tokenEquals(
			list_get(list, list_size(list) - 1),
			token
		);
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
	} else {
		char str[100];
		str[0]='\0';
		
		strcat(str,"Syntax Error ");
		
		strcat(str,"in ");
		
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
	return true;
}

bool list_expect(struct TokenList* list, int token_kind){

	return list_expect_internal(list, makeToken(token_kind));
}

bool list_expect_2(struct TokenList* list, int token_kind, char* token_value){
	return list_expect_internal(list, makeToken2(token_kind, token_value));
}

struct TokenList* list_copy(struct TokenList* other) {

	//DEBUG
	printf("list_copy(...)\n");

	struct TokenList* res = makeTokenList();

	strcpy(res->relPath, other->relPath);
	
	for(int i = 0; i < list_size(other); i++){
		
		list_add(res, list_get(other, i));
	}
	
	return res;
}

void list_set(struct TokenList* list, struct TokenList* copy) {

	//DEBUG
	printf("list_set(...)\n");

	free(list->tokens);
	list->tokens = malloc(sizeof(struct Token*) * copy->capacity);

	list->capacity = copy->capacity;
	list->tokensc = copy->tokensc;

	memcpy(list->tokens, copy->tokens, copy->tokensc * sizeof(struct Token*));

	//DEBUG
	printf("return from list_set\n");
}

struct Token* list_get(struct TokenList* list, int i) {
	return list->tokens[i*sizeof(struct Token*)];
}

struct Token* list_head(struct TokenList* list) {
	return list_get(list, 0);
}

char* list_code(struct TokenList* list, bool debug) {
	//it should be a limited fragment 

	if(debug){
		printf("list_code(...)\n");
	}

	if(list == NULL){
		printf("list == NULL\n");
		exit(1);
	}

	char* str = malloc(sizeof(char)*100);
	if(str == NULL){ return "ERROR"; exit(1);}
	str[0]='\0';

	

	int i=0;
	while(i < list_size(list) && (i < 10)){
		struct Token* tk = list_get(list,i);

		if(debug){
			printf("p1\n");
		}

		strcat(str, tk->value);
		strcat(str, " ");
		i++;
	}
	strcat(str,"    ");
	strcat(str,"[");

	if(debug){
		printf("p2\n");
	}

	i=0;
	while(i < list_size(list) && (i < 10)){
		struct Token* tk = list_get(list,i);
		char buf[10];
		sprintf(buf, "%d", tk->kind);
		strcat(str, buf);
		strcat(str,",");
		i++;
	}
	strcat(str,"]");

	return str;
}
