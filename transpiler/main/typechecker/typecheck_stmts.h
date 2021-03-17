#ifndef TYPECHECK_STMTS_H
#define TYPECHECK_STMTS_H

void tc_stmt(struct Stmt* s);

void tc_assignstmt(struct AssignStmt* a);
void tc_methodcall(struct MethodCall* m);
void tc_ifstmt(struct IfStmt* i);
void tc_whilestmt(struct WhileStmt* w);
void tc_loopstmt(struct LoopStmt* l);
void tc_retstmt(struct RetStmt* r);
void tc_switchstmt(struct SwitchStmt* s);
void tc_forstmt(struct ForStmt* f);

#endif
