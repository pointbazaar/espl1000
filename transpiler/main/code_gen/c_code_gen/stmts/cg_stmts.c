#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "typeinference/typeinfer.h"

#include "cg_stmts.h"
#include "cg_throwstmt.h"
#include "cg_assignstmt.h"
#include "cg_call.h"
#include "cg_ifstmt.h"
#include "cg_whilestmt.h"
#include "cg_loopstmt.h"
#include "cg_forstmt.h"
#include "cg_trycatch.h"
#include "cg_switchcase.h"
#include "code_gen/c_code_gen/const/cg_const.h"
#include "code_gen/c_code_gen/var/cg_var.h"
#include "code_gen/c_code_gen/expr/cg_expr.h"
#include "code_gen/c_code_gen/types/cg_types.h"

#include "code_gen/c_code_gen/cg.h"

#include "code_gen/util/indent.h"
#include "code_gen/types/gen_c_types.h"
#include "code_gen/structs/structs_code_gen.h"

#include "tables/sst/sst.h"
#include "tables/sst/sst_prefill.h"
#include "tables/stst/stst.h"
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
			if(s->isBreak){ transpileBreakStmt(ctx);  }
			if(s->isContinue){ transpileContinueStmt(ctx); }
			if(s->isThrow){ transpileThrowStmt(ctx); }
			break;
		
		default:
			printf("Error in transpileStmt\n");
			exit(1);
	}
	
	fprintf(ctx->file, "\n");
}

//stmt related

void transpileRetStmt(struct RetStmt* rs, struct Ctx* ctx){
	
	indent(ctx);
	
	fprintf(ctx->file, "return ");
	transpileExpr(rs->returnValue, ctx);
}

void transpileBreakStmt(struct Ctx* ctx){
	indent(ctx); fprintf(ctx->file, "break;");
}

void transpileContinueStmt(struct Ctx* ctx){
	indent(ctx); fprintf(ctx->file, "continue;");
}



