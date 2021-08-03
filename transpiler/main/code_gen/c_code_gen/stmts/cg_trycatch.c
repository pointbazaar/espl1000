#include <stdio.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "../cg.h"

#include "code_gen/util/indent.h"

void transpileTryCatchStmt(struct TryCatchStmt* tcs, struct Ctx* ctx){
	
	ctx->index_try_stmt += 1;
	uint16_t index = ctx->index_try_stmt;
	
	ctx->in_try_block = true;
	
	//declare jmp_buf _jb$NUM;
	indent(ctx); fprintf(ctx->file, "jmp_buf _jb%d; \n", index);
	indent(ctx); fprintf(ctx->file, "if(setjmp(_jb%d) == 0) ", index);
	
	//all calls in this block must
	//have that jmp_buf pointer as argument
	transpileStmtBlock(tcs->try_block, ctx);
	
	ctx->in_try_block = false;
	
	indent(ctx); fprintf(ctx->file, " else ");
	
	transpileStmtBlock(tcs->catch_block, ctx);
}
