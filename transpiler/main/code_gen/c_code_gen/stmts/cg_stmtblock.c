#include <stdio.h>

#include "ast/ast.h"

#include "../_cg.h"
#include "../../../util/ctx.h"
#include "../../util/indent.h"

void transpileStmtBlock(struct StmtBlock* block, struct Ctx* ctx){

    fprintf(ctx->file, "{\n");

    for(int i=0; i < block->count; i++){
        (ctx->indent_level) += 1;
        transpileStmt(block->stmts[i], ctx);
        (ctx->indent_level) -= 1;
    }

    indent(ctx);
    fprintf(ctx->file, "}\n");
}
