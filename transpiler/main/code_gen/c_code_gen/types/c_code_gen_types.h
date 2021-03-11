#ifndef C_CODE_GEN_TYPES_H
#define C_CODE_GEN_TYPES_H


void transpileType(struct Type* t, struct Ctx* ctx);
void transpileBasicTypeWrapped(struct BasicTypeWrapped* btw, struct Ctx* ctx);
void transpileTypeParam(struct TypeParam* tp, struct Ctx* ctx);
void transpileArrayType(struct ArrayType* atype, struct Ctx* ctx);

void transpileSimpleType(struct SimpleType* simpleType, struct Ctx* ctx);
void transpileSubrType(struct SubrType* subrType, struct Ctx* ctx);


#endif
