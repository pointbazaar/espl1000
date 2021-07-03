#ifndef TYPECHECK_STMTS_H
#define TYPECHECK_STMTS_H

void tc_stmt(struct Stmt* s, struct TCCtx* tcctx);

void tc_ifstmt(struct IfStmt* i, struct TCCtx* tcctx);
void tc_whilestmt(struct WhileStmt* w, struct TCCtx* tcctx);
void tc_loopstmt(struct LoopStmt* l, struct TCCtx* tcctx);
void tc_retstmt(struct RetStmt* r, struct TCCtx* tcctx);
void tc_switchstmt(struct SwitchStmt* s, struct TCCtx* tcctx);
void tc_forstmt(struct ForStmt* f, struct TCCtx* tcctx);

#endif
