#include <stdio.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "../cg.h"
#include "../_cg.h"

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
