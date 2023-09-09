#ifndef TYPE_STR_H
#define TYPE_STR_H

#include "ast/ast.h"

struct Type* typeFromStrPrimitive(struct ST* st, char* typeName);

struct Type* typeFromStrArray(struct ST* st, char* typeName);

// [1]
/*  register the pointer in SymbolTable 'st'
 *	so it can manage that memory,
 *	as this pointer is not part of the AST Tree
 */
void registerInferredType(struct ST* st, struct Type* t);

#endif
