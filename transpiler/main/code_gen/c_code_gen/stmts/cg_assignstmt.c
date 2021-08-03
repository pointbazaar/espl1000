#include <stdio.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "../types/cg_types.h"
#include "cg_assignstmt.h"
#include "../var/cg_var.h"
#include "../expr/cg_expr.h"

#include "code_gen/util/indent.h"

#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

void transpileAssignStmt(struct AssignStmt* as, struct Ctx* ctx){

	indent(ctx);
	
	//if we assign a function pointer
	bool isSubrType = false;

	if(as->opt_type != NULL){
		
		//is it a function pointer?
		if(as->opt_type->m1 != NULL){
			
			if(as->opt_type->m1->subr_type != NULL){
				
				isSubrType = true;
				
				strncpy(
					ctx->current_function_pointer_var_or_arg_name,
					
					//we know that it is a simple
					//variable (without index)because 
					//structures and arrays, 
					//would have no type
					//definition in front,
					//as they already have a known type
					as->var->simple_var->name,
					DEFAULT_STR_SIZE
				);
			}
		}
		
		transpileType(as->opt_type, ctx);
		fprintf(ctx->file, " ");
		
	}else if(as->opt_type == NULL && as->var->member_access == NULL){
		//find type via local variable symbol table
		
		struct LVSTLine* line = lvst_get(
			ctx->tables->lvst, as->var->simple_var->name
		);

		if(line->first_occur == as){
			
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
