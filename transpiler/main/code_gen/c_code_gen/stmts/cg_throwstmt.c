#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "cg_throwstmt.h"

#include "code_gen/util/indent.h"

void transpileThrowStmt(struct Ctx* ctx){
	
	//the correct jmp_buf 
	
	//is either the one of the current try-block
	if(ctx->in_try_block){
		indent(ctx); 
		fprintf(ctx->file, "longjmp(_jb%d,1);", ctx->index_try_stmt);
		return;
	}
	
	bool current_subr_throws = true;
	
	if(current_subr_throws){
		//or the one given as argument to the function 
		indent(ctx);
		fprintf(ctx->file, "longjmp(*_jb,1);");
		return;
	}
	
	printf("error in transpileThrowStmt\n");
	exit(1);
}
