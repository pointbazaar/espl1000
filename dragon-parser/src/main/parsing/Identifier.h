#ifndef IDENTIFIER
#define IDENTIFIER

#include <stdbool.h>

struct TokenList;

struct Identifier  {

	char* identifier;
};

struct Identifier* makeIdentifier(struct TokenList* tokens, bool debug);

#endif