#ifndef IDENTIFIER
#define IDENTIFIER

#include <stdbool.h>

struct Identifier;
struct TokenList;

struct Identifier* makeIdentifier(struct TokenList* tokens);

#endif
