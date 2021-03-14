
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "typeinference/typeinfer.h"

#include "c_code_gen_const.h"

#include "code_gen/util/code_gen_util.h"
#include "code_gen/types/gen_c_types.h"
#include "code_gen/structs/structs_code_gen.h"

#include "analyzer/fn_analyzer.h"
#include "analyzer/lv_analyzer.h"

#include "tables/sst/sst.h"
#include "tables/sst/sst_prefill.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

void transpileBoolConst(struct BoolConst* bc, struct Ctx* ctx){
	fprintf(ctx->file, (bc->value)?"true":"false");
}

void transpileIntConst(struct IntConst* ic, struct Ctx* ctx){
	fprintf(ctx->file, "%d", ic->value);
}

void transpileHexConst(struct HexConst* hc, struct Ctx* ctx){
	fprintf(ctx->file, "0x%x", hc->value);
}

void transpileBinConst(struct BinConst* hc, struct Ctx* ctx){
	
	fprintf(ctx->file, "0b");
	
	uint32_t value = hc->value;
	
	if(value == 0){
		fprintf(ctx->file, "0");
		return;
	}
	
	const int size = 128;
	
	char buffer[size];
	
	int index = size - 1;
	buffer[index--] = '\0';
	
	while(value > 0){
		
		uint8_t bit = value & 0x1;
		
		buffer[index--] = (bit == 0x1) ? '1' : '0';
		
		value >>= 1;
	}
	
	fprintf(ctx->file, "%s", buffer+index+1);
}

void transpileCharConst(struct CharConst* cc, struct Ctx* ctx){
	fprintf(ctx->file, "'%c'", cc->value);
}

void transpileFloatConst(struct FloatConst* fc, struct Ctx* ctx){
	fprintf(ctx->file, "%f", fc->value);
}

void transpileStringConst(struct StringConst* s, struct Ctx* ctx){
	fprintf(ctx->file, "%s", s->value);
}
