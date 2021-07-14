#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "typeinference/typeinfer.h"

#include "cg_var.h"
#include "code_gen/c_code_gen/expr/cg_expr.h"

#include "code_gen/util/indent.h"
#include "code_gen/types/gen_c_types.h"
#include "code_gen/structs/structs_code_gen.h"

void transpileVariable(struct Variable* var, struct Ctx* ctx){
               
       transpileSimpleVar(var->simple_var, ctx);
       
       if(var->member_access != NULL){
               fprintf(ctx->file, "->");
               transpileVariable(var->member_access, ctx);
       }
}

void transpileSimpleVar(struct SimpleVar* svar, struct Ctx* ctx){
               
       assert(svar->name != NULL);
       
       fprintf(ctx->file, "%s", svar->name);
       
       for(int i=0;i < svar->count_indices; i++){
               fprintf(ctx->file, "[");
               transpileExpr(svar->indices[i], ctx);
               fprintf(ctx->file, "]");
       }
}
