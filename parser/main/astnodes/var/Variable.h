#ifndef VARIABLE
#define VARIABLE

#include <stdbool.h>

struct TokenList;
struct Variable;

// @returns NULL on error
struct Variable* makeVariable(struct TokenList* tokens);

#endif
