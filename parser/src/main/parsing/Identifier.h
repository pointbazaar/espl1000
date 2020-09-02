#ifndef IDENTIFIER
#define IDENTIFIER

#include <stdbool.h>

#include "../../../../ast/ast.h"

struct Identifier* makeIdentifier(struct TokenList* tokens, bool debug);

#endif
