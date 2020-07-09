#ifndef SIMPLETYPE
#define SIMPLETYPE

#include <stdbool.h>

struct TokenList;

#include "../../../../../ast/ast.h"
struct SimpleType* makeSimpleType2(struct TokenList* tokens, bool debug);
struct SimpleType* makeSimpleType(char* typeName);

#endif
