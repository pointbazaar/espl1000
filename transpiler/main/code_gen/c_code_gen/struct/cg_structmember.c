#include <string.h>

#include "util/ctx.h"

#include "../_cg.h"

void transpileStructMember(struct StructMember* m, struct Ctx* ctx){
	
	fprintf(ctx->file, "\t");
	
	bool isSubrType = false;
	//is it a function pointer?
	if(m->type->basic_type != NULL){
		if(m->type->basic_type->subr_type != NULL){
			isSubrType = true;
			strncpy(
				ctx->current_function_pointer_var_or_arg_name,
				m->name,
				DEFAULT_STR_SIZE
			);
		}
	}
	
	transpileType(m->type, ctx);
	
	if(!isSubrType){
		//with C function pointers, the identifier of the 
		//function pointer is between the c_types_util
		//describing it
		fprintf(ctx->file, " %s", m->name);
	}
	
	fprintf(ctx->file, ";\n");
}
