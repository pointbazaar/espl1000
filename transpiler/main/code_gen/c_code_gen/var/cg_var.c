#include <stdio.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"
#include "flags.h"

#include "typeinference/typeinfer.h"

#include "../_cg.h"

void transpileVariable(struct Variable* var, struct Ctx* ctx){
               
	transpileSimpleVar(var->simple_var, ctx);

	if(var->member_access != NULL){
		fprintf(ctx->file, "->");
		transpileVariable(var->member_access, ctx);
	}
}

void transpileSimpleVar(struct SimpleVar* svar, struct Ctx* ctx){
	fprintf(ctx->file, "%s", svar->name);

	for(int i=0;i < svar->count_indices; i++){
		fprintf(ctx->file, "[");
		transpileExpr(svar->indices[i], ctx);
		fprintf(ctx->file, "]");
	}
}
