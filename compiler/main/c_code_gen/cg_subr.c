#include <stdio.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"
#include "flags.h"

#include "_cg.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"
#include "tables/stst/stst.h"

void transpileMethodSignature(struct Method* m, struct Ctx* ctx){
	
	transpileType(m->decl->return_type, ctx);

	fprintf(ctx->file, " %s(", m->decl->name);

	for(int i=0; i < m->decl->count_args; i++){
		transpileDeclArg(m->decl->args[i], ctx);
		if(i < (m->decl->count_args)-1){
			fprintf(ctx->file, ", ");
		}
	}

	fprintf(ctx->file, ")");
}

void transpileDeclArg(struct DeclArg* da, struct Ctx* ctx){

	bool isSubrType = false;
	//is it a function pointer?
	if(da->type->basic_type != NULL){
		if(da->type->basic_type->subr_type != NULL){
			isSubrType = true;
			strncpy(
				ctx->current_function_pointer_var_or_arg_name,
				da->name,
				DEFAULT_STR_SIZE
			);
		}
	}
	
	transpileType(da->type, ctx);

	if(da->has_name && !isSubrType){
		//if it has a name, and is a subroutine type
		//(function pointer), then the name
		//is transpiled by transpileSubrType
		fprintf(ctx->file, " %s", da->name);
	}
}
