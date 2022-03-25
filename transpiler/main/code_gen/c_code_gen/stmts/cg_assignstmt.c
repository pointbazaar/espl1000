#include <stdio.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "../_cg.h"

#include "code_gen/util/indent.h"

#include "../../../typeinference/typeinfer.h"

#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

void transpileAssignStmt(struct AssignStmt* as, struct Ctx* ctx){

	indent(ctx);
	
	//if we assign a function pointer
	bool isSubrType = false;

	struct Type* left_type = NULL;

	if(as->opt_type != NULL){

	    left_type = as->opt_type;

		//is it a function pointer?
		if(as->opt_type->basic_type != NULL){
			
			if(as->opt_type->basic_type->subr_type != NULL){
				
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

		left_type = line->type;

		if(line->first_occur == as){
			//an assignment to this local variable first occurs in this assignment statement
			transpileType(line->type, ctx);
			fprintf(ctx->file, " ");
		}
	}else{

	    //as->optType == NULL && as->var->memberAccess != 0
        //infer the type of the left side
        left_type = infer_type_variable(ctx->tables, as->var);
	}

	const bool assignToTypeParam = left_type->type_param != NULL;
	
	if(!isSubrType){
		//if it is a subroutine type, in C unfortunately
		//the name of the variable is inbetween the c_types_util.
		transpileVariable(as->var, ctx);
	}
	
	fprintf(ctx->file, " = ");

	//cast to void* when assigning to a type parameter.
    if(assignToTypeParam){
        fprintf(ctx->file, "(void*)");
    }

	transpileExpr(as->expr, ctx);
}
