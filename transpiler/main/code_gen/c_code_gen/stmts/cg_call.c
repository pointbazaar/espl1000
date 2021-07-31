#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "../expr/cg_expr.h"

#include "tables/sst/sst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

void transpileCall(struct Call* mc, struct Ctx* ctx){

	char* subr_name_in_c = mc->name;

	if(sst_contains(ctx->tables->sst, mc->name)) {

		struct SSTLine* line = sst_get(ctx->tables->sst, mc->name);
		if (line->name_in_c != NULL){ subr_name_in_c = line->name_in_c; }
	}

		fprintf(ctx->file, "%s(", subr_name_in_c);

	for(int i=0;i < mc->count_args; i++){
		
		transpileExpr(mc->args[i], ctx);
		
		if(i < (mc->count_args)-1){
			fprintf(ctx->file, ", ");
		}
	}
	
	//find out if function throws
	bool throws = true;
	
	if(sst_contains(ctx->tables->sst, mc->name)){
		
		struct SSTLine* line = sst_get(ctx->tables->sst, mc->name);
		
		throws = line->throws;
		
	}else if (lvst_contains(ctx->tables->lvst, mc->name)){
		
		struct LVSTLine* line2 = lvst_get(ctx->tables->lvst, mc->name);
		
		if(line2->type->m1 == NULL){ exit(1); }
		struct BasicType* bt = line2->type->m1;
		if(bt->subr_type == NULL){ exit(1); }
		
		throws = bt->subr_type->throws;
	} else {
		printf("could not find function %s\n", mc->name);
		exit(1);
	}
	
	//sneak in a 
	//jmp_buf* _jb
	//argument if the relevant function throws
	if(throws){
		if(mc->count_args > 0){
			fprintf(ctx->file, ", ");
		}
		if(ctx->in_try_block){
			fprintf(ctx->file, "&_jb%d", ctx->index_try_stmt);
		}else{
			fprintf(ctx->file, "_jb");
		}
	}

	fprintf(ctx->file, ")");
}
