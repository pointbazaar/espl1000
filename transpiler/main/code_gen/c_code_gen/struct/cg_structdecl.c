#include <stdlib.h>

#include "util/ctx.h"

#include "cg_structdecl.h"
#include "cg_struct_reorder.h"
#include "cg_structmember.h"

void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx){
	
	fprintf(ctx->file ,"struct %s {\n", s->type->structType->typeName);
	
	struct StructMember** reordered = struct_reorder(s);
			
	for(uint32_t i = 0; i < s->count_members; i++){
		
		transpileStructMember(reordered[i], ctx);
	}
	
	free(reordered);
	
	fprintf(ctx->file, "};\n");
}
