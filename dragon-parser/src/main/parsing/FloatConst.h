#ifndef FLOATCONST
#define FLOATCONST

#include <stdbool.h>

#include "../../../../ast/ast.h"

struct FloatConst* makeFloatConst(struct TokenList* tokens, bool debug);

#endif
