#ifndef SIMPLEVAR
#define SIMPLEVAR

struct TokenList;
#include "../../../../ast/ast.h"

struct SimpleVar* makeSimpleVar(struct TokenList* tokens, bool debug);

#endif
