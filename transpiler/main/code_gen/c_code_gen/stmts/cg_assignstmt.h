#ifndef CG_ASSIGNSTMT_H
#define CG_ASSIGNSTMT_H

struct AssignStmt;
struct Ctx;

void transpileAssignStmt(struct AssignStmt* as, struct Ctx* ctx);

#endif
