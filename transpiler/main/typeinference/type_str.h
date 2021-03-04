#ifndef TYPE_STR_H
#define TYPE_STR_H

#include "../../ast/ast.h"

/* type_str.h provides facilities
 * to convert smalldragon types between their 
 * AST (struct Type*, struct ...) Representations
 * and their source-code string Representations
 */

char* typeToStr(struct Type* t);

char* typeToStrBasicTypeWrapped	(struct BasicTypeWrapped* b);
char* typeToStrArrayType		(struct ArrayType* t);
char* typeToStrSubrType			(struct SubrType* t);

//--------------

struct Type* typeFromStr(struct ST* st, char* typeName, bool isPrimitive, bool isIntType);

#endif
