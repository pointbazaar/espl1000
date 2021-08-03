#include <stdio.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "../cg.h"
#include "../_cg.h"

#include "code_gen/util/indent.h"

void transpileSwitchStmt(struct SwitchStmt* s, struct Ctx* ctx){
	
	indent(ctx);
	fprintf(ctx->file, "switch (");
	transpileExpr(s->expr, ctx);
	fprintf(ctx->file, ") {\n");
	
	(ctx->indent_level) += 1;
	for(uint32_t i=0; i < s->count_cases; i++){
		transpileCaseStmt(s->cases[i], ctx);
	}
	(ctx->indent_level) -= 1;
	
	indent(ctx);
	fprintf(ctx->file, "}\n");
}
void transpileCaseStmt(struct CaseStmt* s, struct Ctx* ctx){
	
	indent(ctx);
	fprintf(ctx->file, "case ");

	transpileConstValue(s->const_value, ctx);

	fprintf(ctx->file, ":\n");
	
	if(s->block == NULL){ return; }
	
	indent(ctx);
	transpileStmtBlock(s->block, ctx);
	indent(ctx);
	fprintf(ctx->file, "break;\n");
}
