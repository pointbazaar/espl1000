#include <stdio.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "../_cg.h"

void transpileExpr(struct Expr* expr, struct Ctx* ctx){
	
	transpileUnOpTerm(expr->term1, ctx);

	if(expr->op != NULL){
		transpileOp(expr->op, ctx);
		transpileUnOpTerm(expr->term2, ctx);
	}
}
