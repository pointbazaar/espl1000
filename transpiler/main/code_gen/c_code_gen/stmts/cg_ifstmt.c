#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "cg_ifstmt.h"
#include "../expr/cg_expr.h"
#include "../cg.h"

#include "code_gen/util/indent.h"

void transpileIfStmt(struct IfStmt* is, struct Ctx* ctx){
	
	indent(ctx);
	
	fprintf(ctx->file, "if (");
	transpileExpr(is->condition, ctx);
	
	fprintf(ctx->file, ")");
	transpileStmtBlock(is->block, ctx);
	
	if(is->elseBlock == NULL){ return; }
	
	indent(ctx);
	fprintf(ctx->file, "else");
	transpileStmtBlock(is->elseBlock, ctx);
}
