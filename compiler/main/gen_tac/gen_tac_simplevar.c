#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

static void case_indices(struct TACBuffer* buffer, struct SimpleVar* v, struct Ctx* ctx);
static void case_default(struct TACBuffer* buffer, struct SimpleVar* v, struct Ctx* ctx);

void tac_simplevar(struct TACBuffer* buffer, struct SimpleVar* v, struct Ctx* ctx){

    if(v->count_indices != 0){
        case_indices(buffer, v, ctx);
    }else{
		case_default(buffer, v, ctx);
	}
}

static void case_indices(struct TACBuffer* buffer, struct SimpleVar* v, struct Ctx* ctx){
	
	if(v->count_indices > 1){
		//TODO
		printf("more than 1 indices not yet implemented for avr_code_gen\n");
		exit(1);
	}
	
	//load t1 = x
	//code to calculate index
	//index is now in t2
	//t1 = t1 + t2
	//t4 = [t1]
	
	const uint32_t local_index = lvst_index_of(ctx_tables(ctx)->lvst, v->name);
    
    tacbuffer_append(buffer, makeTACLoadLocal(make_temp(), local_index));
    
    uint32_t t1 = tacbuffer_last_dest(buffer);
    
    tac_expr(buffer, v->indices[0], ctx);
    
    uint32_t t2 = tacbuffer_last_dest(buffer);
    
    tacbuffer_append(buffer, makeTACBinOp(t1, TAC_OP_ADD, t2));
    
    tacbuffer_append(buffer, makeTACLoad(make_temp(), t1));
}

static void case_default(struct TACBuffer* buffer, struct SimpleVar* v, struct Ctx* ctx){
	
	const uint32_t local_index = lvst_index_of(ctx_tables(ctx)->lvst, v->name);
    
    tacbuffer_append(buffer, makeTACLoadLocal(make_temp(), local_index));
}
