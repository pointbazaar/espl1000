#ifndef SMALLDRAGON_TOPLEVEL_GEN_TAC_H
#define SMALLDRAGON_TOPLEVEL_GEN_TAC_H

#include "code_gen/avr/tac/tac.h"
struct TACBuffer;

extern uint32_t label_loop_end;
extern uint32_t label_loop_start;

void tac_retstmt(struct TACBuffer* buffer, struct RetStmt* r);

void tac_ifstmt(struct TACBuffer* buffer, struct IfStmt* s);

void tac_whilestmt(struct TACBuffer* buffer, struct WhileStmt* w);

void tac_assignstmt(struct TACBuffer* buffer, struct AssignStmt* a);

void tac_stmt(struct TACBuffer* buffer, struct Stmt* stmt);

void tac_call(struct TACBuffer* buffer, struct Call* call);

void tac_unopterm(struct TACBuffer* buffer, struct UnOpTerm* t);

void tac_forstmt(struct TACBuffer* buffer, struct ForStmt* f);

void tac_switchstmt(struct TACBuffer* buffer, struct SwitchStmt* ss);

void tac_method(struct TACBuffer* buffer, struct Method* m);

void tac_stmtblock(struct TACBuffer* buffer, struct StmtBlock* block);

void tac_variable(struct TACBuffer* buffer, struct Variable* v);

void tac_simplevar(struct TACBuffer* buffer, struct SimpleVar* sv);

void tac_expr(struct TACBuffer* buffer, struct Expr* expr);

void tac_term(struct TACBuffer* buffer, struct Term* t);

void tac_constvalue(struct TACBuffer* buffer, struct ConstValue* c);

//----
int int_value_from_const(struct ConstValue* cv);

#endif
