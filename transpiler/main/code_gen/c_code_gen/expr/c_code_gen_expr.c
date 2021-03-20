#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "typeinference/typeinfer.h"

#include "c_code_gen_expr.h"

#include "code_gen/c_code_gen/const/c_code_gen_const.h"
#include "code_gen/c_code_gen/stmts/c_code_gen_stmts.h"
#include "code_gen/c_code_gen/var/c_code_gen_var.h"

#include "code_gen/util/code_gen_util.h"
#include "code_gen/types/gen_c_types.h"
#include "code_gen/structs/structs_code_gen.h"

#include "tables/sst/sst.h"
#include "tables/sst/sst_prefill.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

void transpileUnOpTerm(struct UnOpTerm* t, struct Ctx* ctx){
		
	if(t->op != NULL){ transpileOp(t->op, ctx); }
	
	transpileTerm(t->term, ctx);
}

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

void transpileExpr(struct Expr* expr, struct Ctx* ctx){
	
	assert(expr != NULL);
	
	transpileUnOpTerm(expr->term1, ctx);

	if(expr->op != NULL){
		transpileOp(expr->op, ctx);
		transpileUnOpTerm(expr->term2, ctx);
	}
}


void transpileOp(struct Op* op, struct Ctx* ctx){
	fprintf(ctx->file, " %s ", op->op);
}
