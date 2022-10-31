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
	
	//load t1 = x
	
	// //tcurrent=t1
	
	//for index in indices
		//... code to generate index
		//tindex = index 
		//tcurrent = tcurrent + tindex
		//t4 = [tcurrent]
		//tcurrent = t4
	
	const uint32_t local_index = lvst_index_of(ctx_tables(ctx)->lvst, v->name);
    
    tacbuffer_append(buffer, makeTACLoadLocal(make_temp(), local_index));
    
    uint32_t t1 = tacbuffer_last_dest(buffer);
    
    uint32_t tcurrent = t1;
    
    for(int i=0;i < v->count_indices; i++){
		
		//TODO: load the address at the index, instead of
		//simply adding the indices
    
		tac_expr(buffer, v->indices[i], ctx);
		uint32_t tindex = tacbuffer_last_dest(buffer);
		
		tacbuffer_append(buffer, makeTACBinOp(tcurrent, TAC_OP_ADD, tindex));
		
		uint32_t t4 = make_temp();
		tacbuffer_append(buffer, makeTACLoad(t4, tcurrent));
		
		tcurrent = t4;
	}
}

static void case_default(struct TACBuffer* buffer, struct SimpleVar* v, struct Ctx* ctx){
	
	const uint32_t local_index = lvst_index_of(ctx_tables(ctx)->lvst, v->name);
    
    tacbuffer_append(buffer, makeTACLoadLocal(make_temp(), local_index));
}
