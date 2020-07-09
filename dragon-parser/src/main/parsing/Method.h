#ifndef METHOD
#define METHOD

#include <stdbool.h>


struct TokenList;

#include "../../../../ast/ast.h"

struct Method* makeMethod(struct TokenList* tokens, bool debug);

#endif
