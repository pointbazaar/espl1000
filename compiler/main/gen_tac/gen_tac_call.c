#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

void tac_call(struct TACBuffer* buffer, struct Call* call){

    for(size_t i = 0; i < call->count_args; i++){
		
        struct Expr* expr = call->args[i];
        
        tac_expr(buffer, expr);
        
        struct TAC* t = makeTACParam(buffer->buffer[buffer->count-1]->dest);

        tacbuffer_append(buffer, t, true);
    }

    if(call->callable->member_access != NULL){
        printf("member access calls currently unsupported on avr_code_gen\n");
        exit(1);
    }

    if(call->callable->simple_var->count_indices != 0){
        printf("calls with indices currently unsupported on avr_code_gen\n");
        exit(1);
    }
    
    struct TAC* t2 = makeTACCall(make_temp(), call->callable->simple_var->name);

    tacbuffer_append(buffer, t2, true);
}
