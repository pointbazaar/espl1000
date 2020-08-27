#ifndef CHARCONST
#define CHARCONST

#include <stdbool.h>

#include "../../../../ast/ast.h"

struct CharConst* makeCharConst(struct TokenList* tokens,bool debug);

void freeCharConst(struct CharConst* cc);

#endif
