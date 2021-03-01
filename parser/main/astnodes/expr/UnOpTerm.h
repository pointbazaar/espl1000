#ifndef VARIABLE
#define VARIABLE

#include <stdbool.h>

struct TokenList;
struct UnOpTerm;

struct UnOpTerm* makeUnOpTerm(struct TokenList* tokens, bool debug);

#endif
