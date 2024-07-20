#ifndef TOKENLIST
#define TOKENLIST

#include <stdbool.h>
#include <inttypes.h>

struct Token;
struct TokenList;

struct TokenList* makeTokenList();
struct TokenList* makeTokenList2(char* filename);

void list_add(struct TokenList* list, struct Token* token);

void list_consume(struct TokenList* list, int amount);

bool list_starts_with(struct TokenList* list, struct Token* itk);

//they return 'false' if the token is not present
bool list_expect_internal(struct TokenList* list, struct Token* itk);
bool list_expect(struct TokenList* list, int token_kind);

struct TokenList* list_copy(struct TokenList* list);

void list_set(struct TokenList* list, struct TokenList* other);

struct Token* list_get(struct TokenList* list, int i);
struct Token* list_head(struct TokenList* list);
struct Token* list_head_without_annotations(struct TokenList* list);
int list_size(struct TokenList* list);

char* list_code(struct TokenList* list);

void list_print(struct TokenList* list);

char* list_rel_path(struct TokenList* list);

void freeTokenList(struct TokenList* list);
void freeTokenListShallow(struct TokenList* list);

#endif
