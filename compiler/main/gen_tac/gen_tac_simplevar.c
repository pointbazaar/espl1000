#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_simplevar(struct TACBuffer* buffer, struct SimpleVar* v, struct Ctx* ctx){

    if(v->count_indices != 0){
        //TODO
        printf("indices not yet implemented for avr_code_gen\n");
        exit(1);
    }
    
    const uint32_t local_index = lvst_index_of(ctx_tables(ctx)->lvst, v->name);
    
    tacbuffer_append(buffer, makeTACLoadLocal(make_temp(), local_index));
}
