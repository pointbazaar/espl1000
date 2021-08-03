#include <stdlib.h>

#include "util/ctx.h"

#include "../_cg.h"
#include "code_gen/c_code_gen/struct_reorder/cg_struct_reorder.h"

void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx){
	
	fprintf(ctx->file ,"struct %s {\n", s->type->struct_type->type_name);
	
	struct StructMember** reordered = struct_reorder(s);
			
	for(uint32_t i = 0; i < s->count_members; i++){
		
		transpileStructMember(reordered[i], ctx);
	}
	
	free(reordered);
	
	fprintf(ctx->file, "};\n");
}
