#include <stdio.h>
#include <stdlib.h>
#include "ast/ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "../_cg.h"

#include "code_gen/util/indent.h"
#include "code_gen/types/gen_c_types.h"

#include "tables/sst/sst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

void transpileStmt(struct Stmt* s, struct Ctx* ctx){
	
	switch(s->kind){
		
		case 0: transpileLoopStmt(s->ptr.m0, ctx); break;
		
		case 1:
			indent(ctx);
			transpileCall(s->ptr.m1, ctx);
			fprintf(ctx->file, ";");
			break;
		
		case 2: transpileWhileStmt(s->ptr.m2, ctx); break;
		
		case 3: transpileIfStmt(s->ptr.m3, ctx); break;
		
		case 4:
			transpileRetStmt(s->ptr.m4, ctx);
			fprintf(ctx->file, ";");
			break;
		
		case 5:
			transpileAssignStmt(s->ptr.m5, ctx);
			fprintf(ctx->file, ";");
			break;
			
		case 6: transpileTryCatchStmt(s->ptr.m6, ctx); break;
		
		case 7: transpileForStmt(s->ptr.m7, ctx); break;
		
		case 8: transpileSwitchStmt(s->ptr.m8, ctx); break;

		case 99:
			if(s->is_break){ transpileBreakStmt(ctx);  }
			if(s->is_continue){ transpileContinueStmt(ctx); }
			if(s->is_throw){ transpileThrowStmt(ctx); }
			break;
		
		default:
			printf("Error in transpileStmt\n");
			exit(1);
	}
	
	fprintf(ctx->file, "\n");
}

void transpileRetStmt(struct RetStmt* rs, struct Ctx* ctx){
	
	indent(ctx);
	
	fprintf(ctx->file, "return ");
	transpileExpr(rs->return_value, ctx);
}

void transpileBreakStmt(struct Ctx* ctx){
	indent(ctx); fprintf(ctx->file, "break;");
}

void transpileContinueStmt(struct Ctx* ctx){
	indent(ctx); fprintf(ctx->file, "continue;");
}



