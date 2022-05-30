#include <stdio.h>
#include <stdlib.h>

#include "avr_code_gen/tac/tac.h"
#include "avr_code_gen/tacbuffer/tacbuffer.h"
#include "gen_tac.h"

void tac_call(struct TACBuffer* buffer, struct Call* call){

    for(size_t i = 0; i < call->count_args; i++){
        struct Expr* expr = call->args[i];
        tac_expr(buffer, expr);

        struct TAC* t = makeTAC();
        t->kind = TAC_PARAM;
        strcpy(t->arg1, buffer->buffer[buffer->count-1]->dest);
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

    struct TAC* t2 = makeTAC();
    t2->kind = TAC_CALL;
    sprintf(t2->dest, "t%d", make_temp());
    strcpy(t2->arg1, call->callable->simple_var->name);
    tacbuffer_append(buffer, t2, true);
}