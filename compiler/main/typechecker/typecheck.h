#ifndef TYPECHECK_H
#define TYPECHECK_H

struct AST;
struct ST;
#include "tcctx.h"

#include "util/ctx.h"

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

struct TCError* typecheck_ast(struct AST* ast, struct Ctx* ctx, bool print_errors);

#endif
