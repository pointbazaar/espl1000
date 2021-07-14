#ifndef INTCONST
#define INTCONST

#include <stdbool.h>

struct IntConst;
struct TokenList;

struct IntConst* makeIntConst(struct TokenList* tokens);

#endif
