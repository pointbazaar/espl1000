#ifndef TYPE_STR_H
#define TYPE_STR_H

#include "ast/ast.h"

struct Type* typeFromStr(struct ST* st, char* typeName, bool isPrimitive, bool isIntType);

#endif
