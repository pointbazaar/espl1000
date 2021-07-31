#include <stdio.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "cg_whilestmt.h"
#include "../expr/cg_expr.h"
#include "../cg.h"

#include "code_gen/util/indent.h"

void transpileWhileStmt(struct WhileStmt* ws, struct Ctx* ctx){
	
	indent(ctx);
	
	fprintf(ctx->file, "while (");
	
	transpileExpr(ws->condition, ctx);
	
	fprintf(ctx->file, ")");

	transpileStmtBlock(ws->block, ctx);
}
