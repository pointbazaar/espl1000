#include <string.h>

#include "util/ctx.h"
#include "_cg.h"

void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx){
	
	fprintf(ctx->file ,"struct %s {\n", s->type->struct_type->type_name);

	for(uint32_t i = 0; i < s->count_members; i++)
		transpileStructMember(s->members[i], ctx);
	
	fprintf(ctx->file, "};\n");
}

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