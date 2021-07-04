
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "typeinference/typeinfer.h"

#include "c_code_gen_stmts.h"

#include "code_gen/c_code_gen/const/c_code_gen_const.h"
#include "code_gen/c_code_gen/stmts/c_code_gen_stmts.h"
#include "code_gen/c_code_gen/var/c_code_gen_var.h"
#include "code_gen/c_code_gen/expr/c_code_gen_expr.h"
#include "code_gen/c_code_gen/types/c_code_gen_types.h"

#include "code_gen/c_code_gen/c_code_gen.h"

#include "code_gen/util/code_gen_util.h"
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
		
		case 7: transpileForStmt(s->ptr.m7, ctx); break;
		
		case 8: transpileSwitchStmt(s->ptr.m8, ctx); break;

		case 99:
			if(s->isBreak){ transpileBreakStmt(ctx);  }
			if(s->isContinue){ transpileContinueStmt(ctx); }
			break;
		
		default:
			printf("Error in transpileStmt\n");
			exit(1);
	}
	
	fprintf(ctx->file, "\n");
}

//stmt related

void transpileCall(struct Call* mc, struct Ctx* ctx){
	
	fprintf(ctx->file, "%s(", mc->name);

	for(int i=0;i < mc->count_args; i++){
		
		transpileExpr(mc->args[i], ctx);
		
		if(i < (mc->count_args)-1){
			fprintf(ctx->file, ", ");
		}
	}

	fprintf(ctx->file, ")");
}

void transpileWhileStmt(struct WhileStmt* ws, struct Ctx* ctx){
	
	indent(ctx);
	
	fprintf(ctx->file, "while (");
	
	transpileExpr(ws->condition, ctx);
	
	fprintf(ctx->file, ")");

	transpileStmtBlock(ws->block, ctx);
}

void transpileIfStmt(struct IfStmt* is, struct Ctx* ctx){
	
	indent(ctx);
	
	fprintf(ctx->file, "if (");
	transpileExpr(is->condition, ctx);
	
	fprintf(ctx->file, ")");
	transpileStmtBlock(is->block, ctx);
	
	if(is->elseBlock == NULL){ return; }
	
	indent(ctx);
	fprintf(ctx->file, "else");
	transpileStmtBlock(is->elseBlock, ctx);
}

void transpileRetStmt(struct RetStmt* rs, struct Ctx* ctx){
	
	indent(ctx);
	
	fprintf(ctx->file, "return ");
	transpileExpr(rs->returnValue, ctx);
}

void transpileAssignStmt(struct AssignStmt* as, struct Ctx* ctx){

	indent(ctx);
	
	//if we assign a function pointer
	bool isSubrType = false;

	if(as->optType != NULL){
		
		//is it a function pointer?
		if(as->optType->m1 != NULL){
			
			if(as->optType->m1->subrType != NULL){
				
				isSubrType = true;
				
				assert(as->var->simpleVar != NULL);
				
				strncpy(
					ctx->currentFunctionPointerVarOrArgName,
					
					//we know that it is a simple
					//variable (without index)because 
					//structures and arrays, 
					//would have no type
					//definition in front,
					//as they already have a known type
					as->var->simpleVar->name,
					DEFAULT_STR_SIZE
				);
			}
		}
		
		transpileType(as->optType, ctx);
		fprintf(ctx->file, " ");
		
	}else if(as->optType == NULL && as->var->memberAccess == NULL){
		//find type via local variable symbol table
		assert(ctx->tables->lvst != NULL);
		
		struct LVSTLine* line = lvst_get(
			ctx->tables->lvst, as->var->simpleVar->name
		);
		
		assert(line != NULL);
		
		if(line->firstOccur == as){
			
			//an assignment to this local variable first occurs in
			//this assignment statement
			transpileType(line->type, ctx);
			fprintf(ctx->file, " ");
		}
	}
	
	if(!isSubrType){
		//if it is a subroutine type, in C unfortunately
		//the name of the variable is inbetween the types.
		transpileVariable(as->var, ctx);
	}
	
	fprintf(ctx->file, " %s ", as->assign_op);
	transpileExpr(as->expr, ctx);
}

void transpileLoopStmt(struct LoopStmt* ls, struct Ctx* ctx){

	indent(ctx);
	
	unsigned int i = label_count++;
	fprintf(ctx->file, "int count%d = ", i);
	
	transpileExpr(ls->count, ctx);
	
	fprintf(ctx->file, ";\n");
	
	indent(ctx);
	
	fprintf(ctx->file, "while (count%d-- > 0)", i);

	transpileStmtBlock(ls->block, ctx);
}

void transpileBreakStmt(struct Ctx* ctx){
	indent(ctx); fprintf(ctx->file, "break;");
}

void transpileContinueStmt(struct Ctx* ctx){
	indent(ctx); fprintf(ctx->file, "continue;");
}

void transpileForStmt(struct ForStmt* f, struct Ctx* ctx){
		
	indent(ctx);
	
	fprintf(ctx->file, "for (");
	
	fprintf(ctx->file, "long %s = ", f->indexName);
	
	transpileExpr(f->range->start, ctx);
	
	fprintf(ctx->file, ";");
	
	fprintf(ctx->file, "%s <= ", f->indexName);
	
	transpileExpr(f->range->end, ctx);
	
	fprintf(ctx->file, "; %s++)", f->indexName);

	transpileStmtBlock(f->block, ctx);
}
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
