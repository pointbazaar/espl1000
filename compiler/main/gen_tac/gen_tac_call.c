#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "gen_tac.h"

void tac_call(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx){

    for(size_t i = 0; i < call->count_args; i++){
		
        struct Expr* expr = call->args[i];
        
        tac_expr(buffer, expr, ctx);
        
        struct TAC* t = makeTACParam(tacbuffer_last_dest(buffer));

        tacbuffer_append(buffer, t);
    }

    if(call->callable->member_access != NULL){
        printf("member access calls currently unsupported on avr_code_gen\n");
        exit(1);
    }

    if(call->callable->simple_var->count_indices != 0){
        printf("calls with indices currently unsupported on avr_code_gen\n");
        exit(1);
    }
    
    uint32_t index = sst_index_of(ctx_tables(ctx)->sst, call->callable->simple_var->name);
    
    
    struct TAC* t2 = makeTACCall(make_temp(), index);

    tacbuffer_append(buffer, t2);
}
