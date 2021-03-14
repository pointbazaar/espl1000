#ifndef C_CODE_GEN_SUBR_H
#define C_CODE_GEN_SUBR_H


void transpileMethod(struct Method* m, struct Ctx* ctx);
void transpileMethodSignature(struct Method* m, struct Ctx* ctx);
void transpileDeclArg(struct DeclArg* da, struct Ctx* ctx);


#endif
