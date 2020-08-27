#ifndef STRUCTMEMBER
#define STRUCTMEMBER

struct TokenList;
#include "../../../../ast/ast.h"

struct StructMember* makeStructMember(struct TokenList* tokens, bool debug);

void freeStructMember(struct StructMember* sm);

#endif
