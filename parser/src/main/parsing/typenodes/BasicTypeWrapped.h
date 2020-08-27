#ifndef BASICTYPEWRAPPED
#define BASICTYPEWRAPPED

#include <stdbool.h>

struct TokenList;
#include "../../../../../ast/ast.h"
struct BasicTypeWrapped* makeBasicTypeWrappedSimple(struct SimpleType* typeNode);
struct BasicTypeWrapped* makeBasicTypeWrappedSubr(struct SubrType* typeNode);
struct BasicTypeWrapped* makeBasicTypeWrapped2(struct TokenList* tokens, bool debug);

void freeBasicTypeWrapped(struct BasicTypeWrapped* btw);

#endif
