#ifndef FLOATCONST
#define FLOATCONST

#include <stdbool.h>

#include "../../../../ast/ast.h"

struct FloatConst* makeFloatConst(struct TokenList* tokens, bool debug);


void freeFloatConst(struct FloatConst* fc);
#endif
