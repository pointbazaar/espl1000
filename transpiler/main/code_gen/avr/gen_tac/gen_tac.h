#ifndef SMALLDRAGON_TOPLEVEL_GEN_TAC_H
#define SMALLDRAGON_TOPLEVEL_GEN_TAC_H

#include "code_gen/avr/tac/tac.h"

void tac_call(struct TACBuffer* buffer, struct Call* call);

void tac_whilestmt(struct TACBuffer* buffer, struct WhileStmt* w);

void tac_forstmt(struct TACBuffer* buffer, struct ForStmt* f);

void tac_ifstmt(struct TACBuffer* buffer, struct IfStmt* s);

void tac_retstmt(struct TACBuffer* buffer, struct RetStmt* r);

void tac_assignstmt(struct TACBuffer* buffer, struct AssignStmt* a);

void tac_stmt(struct TACBuffer* buffer, struct Stmt* stmt);

void tac_method(struct TACBuffer* buffer, struct Method* m);

void tac_stmtblock(struct TACBuffer* buffer, struct StmtBlock* block);

void tac_expr(struct TACBuffer* buffer, struct Expr* expr);

void tac_unopterm(struct TACBuffer* buffer, struct UnOpTerm* t);

void tac_variable(struct TACBuffer* buffer, struct Variable* v);

void tac_simplevar(struct TACBuffer* buffer, struct SimpleVar* sv);

void tac_term(struct TACBuffer* buffer, struct Term* t);

void tac_constvalue(struct TACBuffer* buffer, struct ConstValue* c);

#endif
