#ifndef TYPE
#define TYPE

#include <stdbool.h>

struct TokenList;
#include "../../../../../ast/ast.h"
struct Type* makeType2(struct TokenList* tokens, bool debug);
struct Type* makeType_1(struct BasicTypeWrapped* typeNode);
struct Type* makeType_2(struct TypeParam* typeNode);
struct Type* makeType_3(struct ArrayType* typeNode);

void freeType(struct Type* t);

#endif
