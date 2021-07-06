#ifndef CG_FORSTMT_H
#define CG_FORSTMT_H

struct ForStmt;
struct Ctx;

void transpileForStmt(struct ForStmt* f, struct Ctx* ctx);

#endif
