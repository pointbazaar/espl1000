#ifndef TYPE_STR_H
#define TYPE_STR_H

#include "../../ast/ast.h"

char* typeToStr(struct Type* t);

char* typeToStrBasicTypeWrapped		(struct BasicTypeWrapped* b);

struct Type* typeFromStr(struct ST* st, char* typeName, bool isPrimitive, bool isIntType);

#endif
