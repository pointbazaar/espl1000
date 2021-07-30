#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "typeinference/typeinfer.h"

#include "cg_types.h"

#include "code_gen/util/indent.h"
#include "code_gen/types/gen_c_types.h"
#include "code_gen/structs/structs_code_gen.h"

#include "tables/sst/sst.h"
#include "tables/sst/sst_prefill.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"


void transpileType(struct Type* t, struct Ctx* ctx){
	
	assert(t != NULL);
	
	if(t->m1 != NULL){ transpileBasicType(t->m1, ctx); }
	
	if(t->m2 != NULL){ transpileTypeParam(t->m2, ctx); }
	
	if(t->m3 != NULL){ transpileArrayType(t->m3, ctx); }
}

void transpileBasicType(struct BasicType* btw, struct Ctx* ctx){
		
	assert(btw != NULL);
		
	char* res = basicType2CType(btw, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileTypeParam(struct TypeParam* tp, struct Ctx* ctx){
		
	char* res = typeParam2CType(tp, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileArrayType(struct ArrayType* atype, struct Ctx* ctx){
		
	char* res = arrayType2CType(atype, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileSimpleType(struct SimpleType* simpleType, struct Ctx* ctx){
		
	char* res = simpleType2CType(simpleType, ctx->tables->stst);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileSubrType(struct SubrType* subrType, struct Ctx* ctx){
	
	char* res = subrType2CType(subrType, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}
