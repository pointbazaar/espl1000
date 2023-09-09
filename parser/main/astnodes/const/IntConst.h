#ifndef INTCONST
#define INTCONST

#include <inttypes.h>
#include <stdbool.h>

struct TokenList;

int32_t makeIntConst(struct TokenList* tokens, bool* error);

#endif
