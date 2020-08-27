#ifndef ARRAYTYPE
#define ARRAYTYPE

#include <stdbool.h>

struct TokenList;
#include "../../../../../ast/ast.h"
struct ArrayType* makeArrayType(struct Type* element_type);
struct ArrayType* makeArrayType2(struct TokenList* tokens, bool debug);

void freeArrayType(struct ArrayType* at);

#endif
