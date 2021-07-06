#ifndef CG_SWITCHCASE_H
#define CG_SWITCHCASE_H

struct SwitchStmt;
struct CaseStmt;
struct Ctx;

void transpileSwitchStmt(struct SwitchStmt* s, struct Ctx* ctx);
void transpileCaseStmt(struct CaseStmt* s, struct Ctx* ctx);

#endif
