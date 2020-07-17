#ifndef METHODCALL
#define METHODCALL

#include <stdbool.h>

struct TokenList;
#include "../../../../../ast/ast.h"
struct MethodCall* makeMethodCall(struct TokenList* tokens,bool debug);

void freeMethodCall(struct MethodCall* mc);

#endif
