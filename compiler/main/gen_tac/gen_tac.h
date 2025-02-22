#pragma once
#include <stdbool.h>

#include "tac/tac.h"
#include "util/ctx.h"

struct TACBuffer;

void tac_retstmt(struct TACBuffer* buffer, struct RetStmt* r, struct Ctx* ctx);

void tac_ifstmt(struct TACBuffer* buffer, struct IfStmt* s, struct Ctx* ctx);

// @returns false on error
bool tac_whilestmt(struct TACBuffer* buffer, struct WhileStmt* w, struct Ctx* ctx);

void tac_assignstmt(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx);

void tac_massignstmt(struct TACBuffer* buffer, struct MAssignStmt* m, struct Ctx* ctx);

// @returns false on error
bool tac_stmt(struct TACBuffer* buffer, struct Stmt* stmt, struct Ctx* ctx);

// @returns false on error
bool tac_call(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx);

bool tac_unopterm(struct TACBuffer* buffer, struct UnOpTerm* t, struct Ctx* ctx);

// @returns false on error
bool tac_forstmt(struct TACBuffer* buffer, struct ForStmt* f, struct Ctx* ctx);

void tac_switchstmt(struct TACBuffer* buffer, struct SwitchStmt* ss, struct Ctx* ctx);

void tac_method(struct TACBuffer* buffer, struct Method* m, struct Ctx* ctx);

void tac_stmtblock(struct TACBuffer* buffer, struct StmtBlock* block, struct Ctx* ctx);

void tac_variable(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx);

// @param last_type_width    used for the final load. can pass NULL here if dont care
void tac_variable_addr(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx, uint8_t* last_type_width);

void tac_simplevar(struct TACBuffer* buffer, struct SimpleVar* sv, struct Ctx* ctx);
void tac_simplevar_addr(struct TACBuffer* buffer, struct SimpleVar* sv, struct Ctx* ctx);

bool tac_expr(struct TACBuffer* buffer, struct Expr* expr, struct Ctx* ctx);

bool tac_term(struct TACBuffer* buffer, struct Term* t, struct Ctx* ctx);

void tac_mdirect(struct TACBuffer* buffer, struct MDirect* m, struct Ctx* ctx);

void tac_constvalue(struct TACBuffer* buffer, struct ConstValue* c);

//----
int int_value_from_const(struct ConstValue* cv);
