#ifndef TOKENLIST
#define TOKENLIST

#include <string.h>

#include "Token.h"

struct TokenList {

	//relative path of the source file
	char* relPath;

	struct Token** tokens;
	int tokensc;

};

struct TokenList* makeTokenList_3(struct Token** result, int resultc, char* sourceFile);
struct TokenList* makeTokenList();

void list_add(struct TokenList* tknList,struct Token* token);
//void list_add(struct TokenList* tknList,int token_kind);
//void list_add(struct TokenList* tknList,int token_kind, char* token_value);

void list_addAll(struct TokenList* tknList,struct Token** tokens, int tokensc);

bool list_consume(struct TokenList* tknList,int n);

bool list_startsWith(struct TokenList* tknList, struct Token* itk);
bool list_endsWith(struct TokenList* tknList, struct Token* itk);

char* wrap(char* s, char* wrap);

//they return 'false' if the token is not present
bool list_expect_internal(struct TokenList* tknList,struct Token* itk);
bool list_expect(struct TokenList* tknList,int token_kind);
bool list_expect_2(struct TokenList* tknList,int token_kind, char* token_value);

struct TokenList* list_copy(struct TokenList* tknList);

void list_set(struct TokenList* tknList,struct TokenList* other);

struct Token* list_get(struct TokenList* tknList,int i);
struct Token* list_head(struct TokenList* tknList);
int list_size(struct TokenList* tknList);

char* list_code(struct TokenList* tknList);

#endif