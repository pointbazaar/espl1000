#ifndef BOOLCONST
#define BOOLCONST

#include <stdbool.h>
#include "../../../../ast/ast.h"


struct BoolConst* makeBoolConst(struct TokenList* tokens, bool debug);

void freeBoolConst(struct BoolConst* bc);	
#endif
