#include <stdio.h>
#include <stdlib.h>

#include "code_gen/avr/tac/tac.h"
#include "code_gen/avr/tacbuffer/tacbuffer.h"
#include "gen_tac.h"

void tac_assignstmt(struct TACBuffer* buffer, struct AssignStmt* a){

    tac_expr(buffer, a->expr);

    if(a->var->member_access != NULL){
        printf("member access assignments currently unsupported on avr\n");
        exit(1);
    }

    if(a->var->simple_var->count_indices != 0){
        printf("assignments with indices currently unsupported on avr\n");
        exit(1);
    }

    struct TAC* t = makeTAC();
    t->kind = TAC_COPY;
    strcpy(t->dest, a->var->simple_var->name);
    strcpy(t->arg1, buffer->buffer[buffer->count - 1]->dest);

    tacbuffer_append(buffer, t, true);
}