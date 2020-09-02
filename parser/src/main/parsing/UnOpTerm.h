#ifndef VARIABLE
#define VARIABLE

#include <stdbool.h>

struct TokenList;

#include "../../../../ast/ast.h"

struct UnOpTerm* makeUnOpTerm(struct TokenList* tokens, bool debug);

#endif
