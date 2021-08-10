#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "../_cg.h"

#include "tables/sst/sst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

//private to this file
struct CallCCodeGenInfo{
	bool throws;
	char* subr_name_in_c;
	bool* expects_type_param;
	bool returns_type_param;
};

static void obtain_c_code_gen_info(struct Call* mc, struct Ctx* ctx, struct CallCCodeGenInfo* info);

static void transpile_call_args(struct Call* mc, struct Ctx* ctx, struct CallCCodeGenInfo* info){

	for(int i=0;i < mc->count_args; i++){

		//cast to void* or uint64_t if the argument expects a type param
		if (info->expects_type_param[i]){
			fprintf(ctx->file, "(uint64_t)");
		}

		transpileExpr(mc->args[i], ctx);

		if(i < (mc->count_args)-1){
			fprintf(ctx->file, ", ");
		}
	}
}

void transpileCall(struct Call* mc, struct Ctx* ctx){

	struct CallCCodeGenInfo info;
	obtain_c_code_gen_info(mc, ctx, &info);

	//TODO: cast to void* or uint64_t if return type is type parameter
	if (info.returns_type_param){
		fprintf(ctx->file, "(uint64_t)");
	}

	fprintf(ctx->file, "%s(", info.subr_name_in_c);

	transpile_call_args(mc, ctx, &info);
	
	//sneak in a 
	//jmp_buf* _jb
	//argument if the relevant function throws
	if(info.throws){
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

	free(info.expects_type_param);
}

static void obtain_c_code_gen_info(struct Call* mc, struct Ctx* ctx, struct CallCCodeGenInfo* info){

	info->expects_type_param = malloc(mc->count_args);
	memset(info->expects_type_param, false, mc->count_args);
	info->subr_name_in_c = mc->name;
	info->returns_type_param = false;

	struct Type* type = NULL;

	if(sst_contains(ctx->tables->sst, mc->name)){

		struct SSTLine* line = sst_get(ctx->tables->sst, mc->name);

		if (line->name_in_c != NULL){ info->subr_name_in_c = line->name_in_c; }

		info->throws = line->throws;
		info->returns_type_param = line->return_type->type_param != NULL;

		if (line->type != NULL){ type = line->type; }

	}else if (lvst_contains(ctx->tables->lvst, mc->name)){

		struct LVSTLine* line2 = lvst_get(ctx->tables->lvst, mc->name);

		if(line2->type->basic_type == NULL){ exit(1); }
		struct BasicType* bt = line2->type->basic_type;
		if(bt->subr_type == NULL){ exit(1); }

		info->throws = bt->subr_type->throws;
		info->returns_type_param = line2->type->type_param != NULL;

		if (line2->type != NULL){ type = line2->type; }
	}

	if (type != NULL){
		if (type->basic_type != NULL && type->basic_type->subr_type != NULL){
			struct SubrType* subr_type = type->basic_type->subr_type;

			for (int i = 0; i < subr_type->count_arg_types; ++i) {
				struct Type* arg_type = subr_type->arg_types[i];
				info->expects_type_param[i] = arg_type->type_param != NULL;
			}
		}
	}
}