#ifndef CG_TRYCATCH_H
#define CG_TRYCATCH_H

struct TryCatchStmt;
struct Ctx;

void transpileTryCatchStmt(struct TryCatchStmt* tcs, struct Ctx* ctx);

#endif
