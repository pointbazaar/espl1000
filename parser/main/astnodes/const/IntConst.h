#ifndef INTCONST
#define INTCONST

#include <stdbool.h>
#include <inttypes.h>

struct TokenList;

int32_t makeIntConst(struct TokenList* tokens, bool* error);

#endif
