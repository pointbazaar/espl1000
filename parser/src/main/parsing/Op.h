#ifndef OP
#define OP

struct TokenList;

#include "../../../../ast/ast.h"

struct Op* makeOp(struct TokenList* tokens, bool debug);

#endif
