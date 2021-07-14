#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "cg_forstmt.h"
#include "../expr/cg_expr.h"
#include "../cg.h"

#include "code_gen/util/indent.h"

void transpileForStmt(struct ForStmt* f, struct Ctx* ctx){
		
	indent(ctx);
	
	fprintf(ctx->file, "for (");
	
	fprintf(ctx->file, "long %s = ", f->index_name);
	
	transpileExpr(f->range->start, ctx);
	
	fprintf(ctx->file, ";");
	
	fprintf(ctx->file, "%s <= ", f->index_name);
	
	transpileExpr(f->range->end, ctx);
	
	fprintf(ctx->file, "; %s++)", f->index_name);

	transpileStmtBlock(f->block, ctx);
}
