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

char* typeParam2CType(struct TypeParam* typeParam, struct Ctx* ctx);

char* basicType2CType(struct BasicType* btw, struct Ctx* ctx);
//----------------------------
bool isIntType(struct Type* t);
//----------------------------
char* typeNameToCFormatStr(char* typeName);
#endif
