#ifndef TOKENLIST
#define TOKENLIST

#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

struct Token;

struct TokenList {
	//relative path of the source file
	//this is part of the struct and not a 'char*'
	//on purpose. to reduce indirection
	//and make the program simpler.
	char rel_path[100];

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

struct TokenList* makeTokenList();

void list_add(struct TokenList* list, struct Token* token);

void list_consume(struct TokenList* list, int amount);

bool list_starts_with(struct TokenList* list, struct Token* itk);

char* wrap(char* s, char* wrap);

//they return 'false' if the token is not present
bool list_expect_internal(struct TokenList* list, struct Token* itk);
bool list_expect(struct TokenList* list, int token_kind);
bool list_expect_2(struct TokenList* list, struct Token* tk);

struct TokenList* list_copy(struct TokenList* list);

void 			list_set(struct TokenList* list, struct TokenList* other);

struct Token* 	list_get(struct TokenList* list, int i);
struct Token* 	list_head(struct TokenList* list);
struct Token* 	list_head_without_annotations(struct TokenList* list);
int 			list_size(struct TokenList* list);

char* list_code(struct TokenList* list);

void list_print(struct TokenList* list);

void freeTokenList(struct TokenList* list);
void freeTokenListShallow(struct TokenList* list);

#endif
