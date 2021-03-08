#ifndef TYPE_STR_H
#define TYPE_STR_H

#include "ast/ast.h"

struct Type* typeFromStrPrimitive(struct ST* st, char* typeName);

struct Type* typeFromStr(struct ST* st, char* typeName);

#endif
