#include <stdio.h>
#include <assert.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "cg_expr.h"
#include "cg_op.h"
#include "cg_unopterm.h"

void transpileExpr(struct Expr* expr, struct Ctx* ctx){
	
	assert(expr != NULL);
	
	transpileUnOpTerm(expr->term1, ctx);

	if(expr->op != NULL){
		transpileOp(expr->op, ctx);
		transpileUnOpTerm(expr->term2, ctx);
	}
}
