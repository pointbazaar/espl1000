#include <string.h>

#include "util/ctx.h"

#include "../types/cg_types.h"
#include "cg_structmember.h"

void transpileStructMember(struct StructMember* m, struct Ctx* ctx){
	
	fprintf(ctx->file, "\t");
	
	bool isSubrType = false;
	//is it a function pointer?
	if(m->type->m1 != NULL){
		if(m->type->m1->subr_type != NULL){
			isSubrType = true;
			strncpy(
				ctx->currentFunctionPointerVarOrArgName,
				m->name,
				DEFAULT_STR_SIZE
			);
		}
	}
	
	transpileType(m->type, ctx);
	
	if(!isSubrType){
		//with C function pointers, the identifier of the 
		//function pointer is between the types
		//describing it
		fprintf(ctx->file, " %s", m->name);
	}
	
	fprintf(ctx->file, ";\n");
}
