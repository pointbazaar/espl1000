#ifndef VARIABLE
#define VARIABLE

#include <stdbool.h>

struct TokenList;

#include "../../../../ast/ast.h"

struct Variable* makeVariable(struct TokenList* tokens, bool debug);

void freeVariable(struct Variable* var);

#endif
