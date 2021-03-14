#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "c_code_gen_subr.h"

#include "../c_code_gen.h"

#include "code_gen/c_code_gen/types/c_code_gen_types.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"
#include "tables/stst/stst.h"

#include "analyzer/lv_analyzer.h"

#include "code_gen/util/code_gen_util.h"
#include "code_gen/types/gen_c_types.h"
#include "code_gen/structs/structs_code_gen.h"

void transpileMethod(struct Method* m, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileMethod\n"); }
	
	//create the local variable symbol table
	lvst_clear(ctx->tables->lvst);
	lvst_fill(m, ctx->tables, ctx->flags->debug);

	transpileMethodSignature(m, ctx);

	transpileStmtBlock(m->block, ctx);
}

void transpileMethodSignature(struct Method* m, struct Ctx* ctx){
	
	transpileType(m->returnType, ctx);

	fprintf(ctx->file, " %s(", m->name);

	for(int i=0; i < m->count_args; i++){
		transpileDeclArg(m->args[i], ctx);
		if(i < (m->count_args)-1){
			fprintf(ctx->file, ", ");
		}
	}

	fprintf(ctx->file, ")");
}

void transpileDeclArg(struct DeclArg* da, struct Ctx* ctx){
		
	assert(da != NULL);
		
	bool isSubrType = false;
	//is it a function pointer?
	if(da->type->m1 != NULL){
		if(da->type->m1->subrType != NULL){
			isSubrType = true;
			strncpy(
				ctx->currentFunctionPointerVarOrArgName,
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
