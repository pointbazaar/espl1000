#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"

#include "util/ctx.h"
#include "flags.h"

#include "_cg.h"

#include "c_code_gen/c_types_util/gen_c_types.h"

#include "tables/sst/sst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

void transpileType(struct Type* t, struct Ctx* ctx){

	if(t->basic_type != NULL){ transpileBasicType(t->basic_type, ctx); }
	
	if(t->type_param != NULL){ transpileTypeParam(t->type_param, ctx); }
	
	if(t->array_type != NULL){ transpileArrayType(t->array_type, ctx); }
}

void transpileBasicType(struct BasicType* btw, struct Ctx* ctx){

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
