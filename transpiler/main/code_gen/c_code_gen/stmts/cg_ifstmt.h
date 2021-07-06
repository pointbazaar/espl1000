#ifndef CG_IFSTMT_H
#define CG_IFSTMT_H

struct IfStmt;
struct Ctx;

void transpileIfStmt(struct IfStmt* is, struct Ctx* ctx);

#endif
