#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typeinference/typeinfer.h>
#include <ast/util/str_ast.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "../_cg.h"

#include "tables/sst/sst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

//private to this file
struct CallCCodeGenInfo{
    char* subr_name_in_c;
	bool* expects_type_param;
	bool returns_type_param;
	bool is_simple_call;
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

	if (info.returns_type_param){
		fprintf(ctx->file, "(uint64_t)");
	}

	if(info.is_simple_call){
        fprintf(ctx->file, "%s", info.subr_name_in_c);
    }else{
        transpileVariable(mc->callable, ctx);
	}

    fprintf(ctx->file, "(");

	transpile_call_args(mc, ctx, &info);

	fprintf(ctx->file, ")");

	free(info.expects_type_param);
}

static void obtain_c_code_gen_info(struct Call* mc, struct Ctx* ctx, struct CallCCodeGenInfo* info){

	info->expects_type_param = malloc(mc->count_args);
	memset(info->expects_type_param, false, mc->count_args);
	info->subr_name_in_c = mc->callable->simple_var->name;
	info->returns_type_param = false;
	info->is_simple_call = false;

	struct Type* type = NULL;

	//set:
	/*
	info->subr_name_in_c;
	info->throws;
	type;
	info->returns_type_param;
	 */

	//NEW CODE
	type = infer_type_variable(ctx->tables, mc->callable);

	if(type->basic_type == NULL || type->basic_type->subr_type == NULL){
	    char* snippet = str_call(mc);
        printf("[CodeGen][Error] Fatal (function call %s has wrong type\n", snippet);
        free(snippet);
        exit(1);
	}

	info->returns_type_param = type->basic_type->subr_type->return_type->type_param != NULL;

	if(
        sst_contains(ctx->tables->sst, mc->callable->simple_var->name)
	    && mc->callable->member_access == NULL
	    && mc->callable->simple_var->count_indices == 0
	){
	    info->is_simple_call = true;
	    struct SSTLine* line = sst_get(ctx->tables->sst, mc->callable->simple_var->name);
	    if (line->name_in_c != NULL){ info->subr_name_in_c = line->name_in_c; }
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