#ifndef _TC_H
#define _TC_H

struct TCCtx;

struct Expr;
struct Term;

void tc_assignstmt(struct AssignStmt* a, struct TCCtx* tcctx);

void tc_methodcall(struct Call* m, struct TCCtx* tcctx);

void tc_stmt(struct Stmt* s, struct TCCtx* tcctx);

void tc_ifstmt(struct IfStmt* i, struct TCCtx* tcctx);
void tc_whilestmt(struct WhileStmt* w, struct TCCtx* tcctx);
void tc_retstmt(struct RetStmt* r, struct TCCtx* tcctx);
void tc_switchstmt(struct SwitchStmt* s, struct TCCtx* tcctx);
void tc_forstmt(struct ForStmt* f, struct TCCtx* tcctx);
void tc_trycatchstmt(struct TryCatchStmt* tcs, struct TCCtx* tcctx);
void tc_throwstmt(struct Stmt* s, struct TCCtx* tcctx);


void tc_expr(struct Expr* expr, struct TCCtx* tcctx);
void tc_unopterm(struct UnOpTerm* uot, struct TCCtx* tcctx);
void tc_term(struct Term* term, struct TCCtx* tcctx);

void tc_var(struct Variable* v, struct TCCtx* tcCtx);
void tc_simplevar(struct SimpleVar* v, struct TCCtx* tcctx);

void tc_range(struct Range* r, struct TCCtx* tcctx);

void tc_method(struct Method* m, struct TCCtx* tcctx);

void tc_stmtblock(struct StmtBlock* s, struct TCCtx* tcctx);

#endif