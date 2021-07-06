#ifndef C_CODE_GEN_STRUCT_H
#define C_CODE_GEN_STRUCT_H


void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx);
void transpileStructMember(struct StructMember* m, struct Ctx* ctx);

#endif
