#ifndef C_CODE_GEN_EXPR_H
#define C_CODE_GEN_EXPR_H


void transpileUnOpTerm(struct UnOpTerm* t, struct Ctx* ctx);
void transpileTerm(struct Term* expr, struct Ctx* ctx);
void transpileExpr(struct Expr* expr, struct Ctx* ctx);
void transpileOp(struct Op* op, struct Ctx* ctx);


#endif
