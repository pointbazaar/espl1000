#ifndef SUBROUTINETYPE
#define SUBROUTINETYPE

#include <stdbool.h>

struct TokenList;
#include "../../../../../ast/ast.h"
struct SubrType* makeSubrType(struct TokenList* tokens, bool debug);
struct SubrType* makeSubrType2(struct Type* tn, bool hasSideEffects);


#endif
