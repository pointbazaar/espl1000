#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "cg_expr.h"
#include "../const/cg_const.h"
#include "../stmts/cg_call.h"
#include "../var/cg_var.h"

void transpileTerm(struct Term* t, struct Ctx* ctx){
		
	switch(t->kind){
		
		case 1: transpileBoolConst(t->ptr.m1, ctx); break;
		case 2: transpileIntConst(t->ptr.m2, ctx); break;
		case 3: transpileCharConst(t->ptr.m3, ctx); break;
		case 4: transpileCall(t->ptr.m4, ctx); break;
		case 5: 
			fprintf(ctx->file, "(");
			transpileExpr(t->ptr.m5, ctx); 
			fprintf(ctx->file, ")");
			break;
		case 6: transpileVariable(t->ptr.m6, ctx); break;
		case 7: transpileFloatConst(t->ptr.m7, ctx); break;
		case 8: transpileStringConst(t->ptr.m8, ctx); break;
		case 9: transpileHexConst(t->ptr.m9, ctx); break;
		case 10:transpileBinConst(t->ptr.m10, ctx); break;
		
		default:
			printf("[Transpiler][Error] in transpileTerm\n"); exit(1);
	}
}
