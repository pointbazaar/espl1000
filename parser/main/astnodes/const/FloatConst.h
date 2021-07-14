#ifndef FLOATCONST
#define FLOATCONST

#include <stdbool.h>

struct FloatConst;
struct TokenList;

struct FloatConst* makeFloatConst(struct TokenList* tokens);

#endif
