#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "c_code_gen_struct.h"

#include "code_gen/c_code_gen/types/c_code_gen_types.h"

#include "code_gen/util/code_gen_util.h"
#include "code_gen/types/gen_c_types.h"
#include "code_gen/structs/structs_code_gen.h"

void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx){
	
	fprintf(ctx->file ,"struct %s {\n", s->type->structType->typeName);
	
	for(int i=0;i < s->count_members;i++){
		transpileStructMember(s->members[i], ctx);
	}
	
	fprintf(ctx->file, "};\n");
}

void transpileStructMember(struct StructMember* m, struct Ctx* ctx){
	
	fprintf(ctx->file, "\t");
	
	bool isSubrType = false;
	//is it a function pointer?
	if(m->type->m1 != NULL){
		if(m->type->m1->subrType != NULL){
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
