#pragma once

#include <stdbool.h>

#include "ast/ast_declare.h"

struct TCCtx;

struct Expr;
struct Term;

/*
 * the functions return true if the respective AST Node typechecks,
 * meaning that it is a valid smalldragon program regarding the c_types_util.
 *
 * This is important, so typechecking does not continue if a part of a
 * statement or other construction is already not sound.
 *
 * This also means that type inference should not be used on expressions
 * of which parts already are known not to be sound constructions.
 */

bool tc_assignstmt(struct AssignStmt* a, struct TCCtx* tcctx);
bool tc_local_var_decl_stmt(struct LocalVarDeclStmt* a, struct TCCtx* tcctx);

bool tc_methodcall(struct Call* m, struct TCCtx* tcctx);

bool tc_stmt(struct Stmt* s, struct TCCtx* tcctx, bool must_return);
bool tc_stmt_must_return(struct Stmt* s, struct TCCtx* tcctx);

bool tc_ifstmt(struct IfStmt* i, struct TCCtx* tcctx, bool must_return);
bool tc_whilestmt(struct WhileStmt* w, struct TCCtx* tcctx);
bool tc_retstmt(struct RetStmt* r, struct TCCtx* tcctx);
bool tc_forstmt(struct ForStmt* f, struct TCCtx* tcctx);

bool tc_expr(struct Expr* expr, struct TCCtx* tcctx);
bool tc_unopterm(struct UnOpTerm* uot, struct TCCtx* tcctx);
bool tc_deref(struct Deref* d, struct TCCtx* tcctx);
bool tc_address_of(struct AddressOf* ao, struct TCCtx* tcctx);
bool tc_term(struct Term* term, struct TCCtx* tcctx);

bool tc_var(struct Variable* v, struct TCCtx* tcCtx);
bool tc_simplevar(struct SimpleVar* v, struct TCCtx* tcctx);
bool tc_simplevar_in_struct(struct Type* containing_type, struct SimpleVar* v, struct TCCtx* tcctx);

bool tc_range(struct Range* r, struct TCCtx* tcctx);

bool tc_method(struct Method* m, struct TCCtx* tcctx);

bool tc_namespace(struct Namespace* n, struct TCCtx* tcctx);

bool tc_stmtblock(struct StmtBlock* s, struct TCCtx* tcctx, bool must_return);
