#ifndef STRUCTDECL
#define STRUCTDECL

struct TokenList;

#include "../../../../ast/ast.h"


struct StructDecl* makeStructDecl(struct TokenList* tokens, bool debug);

#endif
