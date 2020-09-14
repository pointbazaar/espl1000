#ifndef GENCTYPES
#define GENCTYPES

struct SimpleType;
struct Type;
struct ArrayType;
struct SubrType;
struct TypeParam;
struct BasicTypeWrapped;

struct Ctx;

char* simpleType2CType(struct SimpleType* simpleType);

char* type2CType(struct Type* type, struct Ctx* ctx);

char* arrayType2CType(struct ArrayType* arrType, struct Ctx* ctx);

char* subrType2CType(struct SubrType* subrType, struct Ctx* ctx);

char* typeParam2CType(struct TypeParam* typeParam, struct Ctx* ctx);

char* basicTypeWrapped2CType(struct BasicTypeWrapped* btw, struct Ctx* ctx);

#endif
