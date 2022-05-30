#ifndef GENCTYPES
#define GENCTYPES

#include "ast/ast.h"
struct STST;

struct Ctx;

char *simpleType2CType(struct SimpleType *s, struct STST *stst);

char* structType2CType(struct StructType* s, struct STST* stst);

char* primitiveType2CType(struct PrimitiveType* p);

char* type2CType(struct Type* type, struct Ctx* ctx);

char* arrayType2CType(struct ArrayType* arrType, struct Ctx* ctx);

char* subrType2CType(struct SubrType* subrType, struct Ctx* ctx);

char* typeParam2CType();

char* basicType2CType(struct BasicType* btw, struct Ctx* ctx);
//----------------------------

#endif
