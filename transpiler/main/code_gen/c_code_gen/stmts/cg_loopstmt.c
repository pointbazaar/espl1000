#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "cg_loopstmt.h"
#include "../expr/cg_expr.h"
#include "../cg.h"

#include "code_gen/util/indent.h"

void transpileLoopStmt(struct LoopStmt* ls, struct Ctx* ctx){

	indent(ctx);
	
	unsigned int i = label_count++;
	fprintf(ctx->file, "int count%d = ", i);
	
	transpileExpr(ls->count, ctx);
	
	fprintf(ctx->file, ";\n");
	
	indent(ctx);
	
	fprintf(ctx->file, "while (count%d-- > 0)", i);

	transpileStmtBlock(ls->block, ctx);
}
