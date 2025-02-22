#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "TokenList.h"
#include "../TokenKeys.h"
#include "../token/token.h"

struct TokenList {
	//relative path of the source file
	char* rel_path;

	//all the tokens, including those
	//already consumed
	struct Token** tokens; //private
	//how many pointers we can store
	//with the currently allocated memory
	uint16_t capacity; //private

	//the index of the first token that was
	//not already consumed
	uint16_t index_head;

	//the amount of tokens stored,
	//even if they have already been consumed
	uint16_t tokens_stored;

	//token count
	uint16_t tokensc; //private
};

struct TokenList* makeTokenList2(char* filename) {

	const int initial_size = 20;

	struct TokenList* res = malloc(sizeof(struct TokenList));

	if (!res) {
		return NULL;
	}

	memset(res, 0, sizeof(struct TokenList));

	res->rel_path = malloc(sizeof(char) * (strlen(filename) + 1));

	if (!res->rel_path) {
		free(res);
		return NULL;
	}

	strcpy(res->rel_path, filename);
	res->tokensc = 0;

	res->tokens = malloc(sizeof(struct Token*) * initial_size);

	if (!res->tokens) {
		return NULL;
	}

	res->index_head = 0;
	res->tokens_stored = 0;

	res->capacity = initial_size;

	return res;
}

struct TokenList* makeTokenList() {
	return makeTokenList2("/dev/null");
}

void list_add(struct TokenList* list, struct Token* token) {

	if ((list->index_head + list->tokensc + 1) > list->capacity) {
		//we don't have enough capacity
		list->capacity = list->capacity * 2;
		list->tokens = realloc(list->tokens, list->capacity * sizeof(struct Token*));
	}

	list->tokens[list->index_head + list->tokensc] = token;
	list->tokensc++;
	list->tokens_stored++;
}

void list_consume(struct TokenList* list, int amount) {

	list->tokensc -= amount;
	list->index_head += amount;
}

int list_size(struct TokenList* list) {
	return list->tokensc;
}

bool list_starts_with(struct TokenList* list, struct Token* itk) {
	//we cannot rely on .equals()
	//we should use interfaces we can rely on classes to implement
	//the class and the content of the token should be the same for them to be the same

	if (list_size(list) == 0) { return false; }

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

bool list_expect(struct TokenList* list, int token_kind) {

	struct Token* tk = makeToken(token_kind);
	const bool res = list_expect_internal(list, tk);
	freeToken(tk);
	return res;
}

struct TokenList* list_copy(struct TokenList* list) {

	struct TokenList* res = makeTokenList();

	if (!res) {
		return NULL;
	}

	free(res->rel_path);
	res->rel_path = malloc(sizeof(char) * (strlen(list->rel_path) + 1));

	if (!res->rel_path) {
		free(res);
		return NULL;
	}

	strcpy(res->rel_path, list->rel_path);

	if (!list_set(res, list)) {
		free(res->rel_path);
		free(res);
		return NULL;
	}

	return res;
}

bool list_set(struct TokenList* list, struct TokenList* copy) {

	free(list->tokens);

	list->tokens = malloc(sizeof(struct Token*) * copy->capacity);

	if (!list->tokens) {
		return false;
	}

	list->capacity = copy->capacity;
	list->tokensc = copy->tokensc;
	list->index_head = copy->index_head;
	list->tokens_stored = copy->tokens_stored;

	size_t size = copy->capacity * sizeof(struct Token*);
	memcpy(list->tokens, copy->tokens, size);

	return true;
}

struct Token* list_get(struct TokenList* list, int i) {

	return list->tokens[list->index_head + i];
}

struct Token* list_head(struct TokenList* list) {
	return list_get(list, 0);
}

struct Token* list_head_without_annotations(struct TokenList* list) {

	int32_t i = 0;
	struct Token* h;

	do {
		h = list_get(list, i);
		i++;

	} while (
	    (h->kind > _ANNOT_START_ && h->kind < _ANNOT_END_) && i < list_size(list));

	return h;
}

char* list_code(struct TokenList* list) {
	//it should be a limited fragment

	char* str = malloc(sizeof(char) * 100);

	if (!str) {
		return NULL;
	}

	strcpy(str, "");

	const uint32_t line_num = list_get(list, 0)->line_num;
	if (list_size(list) > 0) {
		sprintf(str, "line % 4d:\n", line_num);
	}

	uint32_t prevline = line_num;
	for (int i = 0; i < list_size(list) && (i < 10); i++) {
		struct Token* tk = list_get(list, i);

		if (tk->line_num > prevline) {
			strcat(str, "\n");
		}

		prevline = tk->line_num;

		strcat(str, token_str(tk));
		strcat(str, " ");
	}
	strcat(str, "\n");
	strcat(str, "[");

	char buf[100];
	strcpy(buf, "");

	for (int k = 0; k < list_size(list) && (k < 10); k++) {
		struct Token* tk = list_get(list, k);

		sprintf(buf, "%d", tk->kind);

		strcat(str, buf);
		strcat(str, ",");
	}
	strcat(str, "]");

	return str;
}

void list_print(struct TokenList* list) {

	char* str = list_code(list);
	printf("%s\n", str);
	free(str);
}

char* list_rel_path(struct TokenList* list) {
	return list->rel_path;
}

void freeTokenList(struct TokenList* list) {

	//also frees the tokens within,
	//even those already consumed
	for (int i = 0; i < list->tokens_stored; i++) {
		freeToken(list->tokens[i]);
		list->tokens[i] = NULL;
	}
	freeTokenListShallow(list);
}

void freeTokenListShallow(struct TokenList* list) {
	free(list->tokens);
	list->tokens = NULL;

	free(list->rel_path);
	list->rel_path = NULL;

	free(list);
}
