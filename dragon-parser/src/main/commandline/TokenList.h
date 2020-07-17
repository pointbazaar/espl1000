#ifndef TOKENLIST
#define TOKENLIST

#include <string.h>
#include <stdbool.h>

#include "Token.h"

struct TokenList {

	//relative path of the source file
	//this is part of the struct and not a 'char*'
	//on purpose. to reduce indirection
	//and make the program simpler.
	char relPath[100];

	struct Token** tokens;

	//how many pointers we can store
	//with the currently allocated memory
	int capacity;

	//token count
	int tokensc;

};

struct TokenList* makeTokenList_3(struct Token** result, int resultc, char* sourceFile);
struct TokenList* makeTokenList();

void list_add(struct TokenList* tknList,struct Token* token);

void list_consume(struct TokenList* tknList, int amount);

bool list_startsWith(struct TokenList* tknList, struct Token* itk);

char* wrap(char* s, char* wrap);

//they return 'false' if the token is not present
bool list_expect_internal(struct TokenList* tknList,struct Token* itk);
bool list_expect(struct TokenList* tknList,int token_kind);
bool list_expect_2(struct TokenList* tknList, struct Token* tk);

struct TokenList* list_copy(struct TokenList* tknList);

void list_set(struct TokenList* tknList,struct TokenList* other);

struct Token* list_get(struct TokenList* tknList,int i);
struct Token* list_head(struct TokenList* tknList);
int list_size(struct TokenList* tknList);

char* list_code(struct TokenList* tknList, bool debug);

void freeTokenList(struct TokenList* list);
void freeTokenListShallow(struct TokenList* list);
#endif
