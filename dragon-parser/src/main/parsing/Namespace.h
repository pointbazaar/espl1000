#ifndef NAMESPACE
#define NAMESPACE

struct TokenList;
#include "../../../../ast/ast.h"


struct Namespace* makeNamespace(struct TokenList* tokens, char* name, bool debug);

#endif
