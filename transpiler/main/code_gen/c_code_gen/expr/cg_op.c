#include <stdio.h>
#include <assert.h>

#include "ast/ast.h"

#include "util/ctx.h"

void transpileOp(struct Op* op, struct Ctx* ctx){
	fprintf(ctx->file, " %s ", op->op);
}
