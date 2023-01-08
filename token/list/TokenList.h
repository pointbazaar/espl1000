#ifndef TOKENLIST
#define TOKENLIST

#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

struct Token;

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

struct TokenList* makeTokenList();
struct TokenList* makeTokenList2(char* filename);

void list_add(struct TokenList* list, struct Token* token);

void list_consume(struct TokenList* list, int amount);

bool list_starts_with(struct TokenList* list, struct Token* itk);

//they return 'false' if the token is not present
bool list_expect_internal(struct TokenList* list, struct Token* itk);
bool list_expect(struct TokenList* list, int token_kind);

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
