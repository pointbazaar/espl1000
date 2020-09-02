#ifndef TYPEPARAM
#define TYPEPARAM

#include <stdbool.h>

struct TokenList;
#include "../../../../../ast/ast.h"
struct TypeParam* makeTypeParam(struct TokenList* tokens, bool debug);

#endif
