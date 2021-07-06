#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "cg_switchcase.h"
#include "../expr/cg_expr.h"
#include "../cg.h"

#include "code_gen/util/indent.h"

void transpileSwitchStmt(struct SwitchStmt* s, struct Ctx* ctx){
	
	indent(ctx);
	fprintf(ctx->file, "switch (");
	transpileExpr(s->expr, ctx);
	fprintf(ctx->file, ") {\n");
	
	(ctx->indentLevel) += 1;
	for(uint32_t i=0; i < s->count_cases; i++){
		transpileCaseStmt(s->cases[i], ctx);
	}
	(ctx->indentLevel) -= 1;
	
	indent(ctx);
	fprintf(ctx->file, "}\n");
}
void transpileCaseStmt(struct CaseStmt* s, struct Ctx* ctx){
	
	indent(ctx);
	fprintf(ctx->file, "case ");
	
	switch(s->kind){
		case 0: fprintf(ctx->file, "%s", (s->ptr.m1->value)?"true":"false"); break;
		case 1: fprintf(ctx->file, "'%c'", s->ptr.m2->value); break;
		case 2: fprintf(ctx->file, "%d", s->ptr.m3->value); break;
		default:
			printf("ERROR\n"); exit(1);
	}
	fprintf(ctx->file, ":\n");
	
	if(s->block == NULL){ return; }
	
	indent(ctx);
	transpileStmtBlock(s->block, ctx);
	indent(ctx);
	fprintf(ctx->file, "break;\n");
}
