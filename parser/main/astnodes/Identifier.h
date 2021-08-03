#ifndef IDENTIFIER
#define IDENTIFIER

#include <stdbool.h>

struct Id;
struct TokenList;

struct Id* makeIdentifier(struct TokenList* tokens);

#endif
