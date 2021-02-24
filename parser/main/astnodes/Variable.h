#ifndef VARIABLE
#define VARIABLE

#include <stdbool.h>

struct TokenList;
struct Variable;

struct Variable* makeVariable(struct TokenList* tokens, bool debug);

#endif
