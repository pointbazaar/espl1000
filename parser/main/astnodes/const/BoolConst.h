#ifndef BOOLCONST
#define BOOLCONST

#include <stdbool.h>

struct BoolConst;
struct TokenList;

struct BoolConst* makeBoolConst(struct TokenList* tokens, bool debug);

#endif
