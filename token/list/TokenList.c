#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "TokenList.h"
#include "../TokenKeys.h"
#include "../token/token.h"

struct TokenList* makeTokenList() {

	const int initial_size = 20;

	struct TokenList* res = malloc(sizeof(struct TokenList));
	
	strcpy(res->rel_path, "/dev/null");
	res->tokensc = 0;

	res->tokens = malloc(sizeof(struct Token*)*initial_size);
	res->index_head = 0;
	res->tokens_stored = 0;

	res->capacity = initial_size;

	return res;
}

void list_add(struct TokenList* list, struct Token* token) {

	if((list->index_head + list->tokensc + 1) > list->capacity){
		//we don't have enough capacity
		list->capacity = list->capacity * 2;
		list->tokens = realloc(list->tokens, list->capacity * sizeof(struct Token*));
	}
	
	list->tokens[list->index_head + list->tokensc] = token;
	list->tokensc++;
	list->tokens_stored++;
}

void list_consume(struct TokenList* list, int amount) {
	
	list->tokensc    -= amount;
	list->index_head += amount;
}

int list_size(struct TokenList* list) {
	return list->tokensc;
}

bool list_starts_with(struct TokenList* list, struct Token* itk) {
	//we cannot rely on .equals()
	//we should use interfaces we can rely on classes to implement
	//the class and the content of the token should be the same for them to be the same

    if (list_size(list) == 0) { return  false; }

    return token_equals(list_head(list), itk);
}

bool list_expect_internal(struct TokenList* list, struct Token* token) {
	//it expect a token or a kind of token, otherwise throws an exception
	if (list_size(list) == 0) {
		return false;
	}
	if (list_starts_with(list, token)) {
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

struct TokenList* list_copy(struct TokenList* list) {

	struct TokenList* res = makeTokenList();
	list_set(res, list);
	return res;
}

void list_set(struct TokenList* list, struct TokenList* copy) {
	
	free(list->tokens);
	
	list->tokens = malloc(sizeof(struct Token*) * copy->capacity);

	list->capacity = copy->capacity;
	list->tokensc = copy->tokensc;
	list->index_head = copy->index_head;
	list->tokens_stored = copy->tokens_stored;

	size_t size = copy->capacity * sizeof(struct Token*);
	memcpy(list->tokens, copy->tokens, size);
}

struct Token* list_get(struct TokenList* list, int i) {

	return list->tokens[list->index_head + i];
}

struct Token* list_head(struct TokenList* list) {
	return list_get(list, 0);
}

struct Token* list_head_without_annotations(struct TokenList* list){
	
	uint32_t i = 0;
	struct Token* h;
	
	do{
		h = list_get(list, i);
		i++;
	
	} while(
		(h->kind > _ANNOT_START_ && h->kind < _ANNOT_END_)
		&& i < list_size(list)
	);
	
	return h;
}

char* list_code(struct TokenList* list) {
	//it should be a limited fragment

	char* str = malloc(sizeof(char)*100);
	strcpy(str, "");

	if(list_size(list) > 0){
		int line_num = list_get(list, 0)->line_num;
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
	
	char* str = list_code(list);
	printf("%s\n", str);
	free(str);
}

void freeTokenList(struct TokenList* list){
	
	//also frees the tokens within,
	//even those already consumed
	for(int i=0;i < list->tokens_stored; i++){
		freeToken(list->tokens[i]);
	}
	freeTokenListShallow(list);
}

void freeTokenListShallow(struct TokenList* list){
	free(list->tokens);
	free(list);
}
