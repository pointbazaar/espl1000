#ifndef TYPECHECK_H
#define TYPECHECK_H

#include <stdio.h>

#include "ast/ast.h"
#include "tcctx.h"

/* This Module provides Facilities
 * for typechecking smalldragon Programs.
 * 
 * The AST Visitor is not used here because
 * it does not guarantee a particular visiting order
 * for nodes on the same level of depth in the AST.
 * 
 * For typechecking, the order is very important 
 * however, as the following example shows:
 * 
 * uint8 k = 3 + myfunction;
 * 
 * here we should typecheck the right-hand side
 * of the assignment first, before 
 * typechecking the assignment itself,
 * because the type inference would have problems
 * inferring the type of '3 + myfunction'
 */

bool typecheck_ast(struct AST* ast, struct ST* st);

void tc_namespace  (struct Namespace* n, struct TCCtx* tcctx);
void tc_method     (struct Method* m,    struct TCCtx* tcctx);
void tc_stmtblock  (struct StmtBlock* s, struct TCCtx* tcctx);
void tc_range      (struct Range* r,     struct TCCtx* tcctx);

#endif
