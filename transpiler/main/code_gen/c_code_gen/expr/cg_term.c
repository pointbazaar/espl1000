#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "../_cg.h"

static void transpile_expr_wrapped(struct Expr* expr, struct Ctx* ctx){
	fprintf(ctx->file, "(");
	transpileExpr(expr, ctx);
	fprintf(ctx->file, ")");
}

void transpileTerm(struct Term* t, struct Ctx* ctx){
		
	switch(t->kind){
		case  4: transpileCall(t->ptr.m4, ctx); break;
		case  5:
			transpile_expr_wrapped(t->ptr.m5, ctx); break;
		case  6: transpileVariable(t->ptr.m6, ctx); break;
		case  8: transpileStringConst(t->ptr.m8, ctx); break;
		case 12: transpileConstValue(t->ptr.m12, ctx); break;
		default:
			printf("[Transpiler][Error] in transpileTerm\n"); exit(1);
	}
}
