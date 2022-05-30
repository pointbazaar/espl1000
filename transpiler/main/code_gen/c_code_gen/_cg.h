#ifndef SMALLDRAGON__CG_H
#define SMALLDRAGON__CG_H

struct Ctx;

/*
 * this is the internal Header for the functions generating c code.
 *
 * It should only be included in files in the transpiler/main/code_gen/c_code_gen Module
 */

void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx);
void transpileStructMember(struct StructMember* m, struct Ctx* ctx);

void transpileMethodSignature(struct Method* m, struct Ctx* ctx);
void transpileDeclArg(struct DeclArg* da, struct Ctx* ctx);

//Types
void transpileType     (struct Type* t, struct Ctx* ctx);
void transpileBasicType(struct BasicType* btw, struct Ctx* ctx);
void transpileTypeParam(struct TypeParam* tp, struct Ctx* ctx);
void transpileArrayType(struct ArrayType* atype, struct Ctx* ctx);
void transpileSimpleType(struct SimpleType* simpleType, struct Ctx* ctx);
void transpileSubrType  (struct SubrType* subrType, struct Ctx* ctx);


#endif
