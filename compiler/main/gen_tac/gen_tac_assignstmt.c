#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

void tac_assignstmt(struct TACBuffer* buffer, struct AssignStmt* a){

    tac_expr(buffer, a->expr);

    if(a->var->member_access != NULL){
        printf("member access assignments currently unsupported on avr_code_gen\n");
        exit(1);
    }

    if(a->var->simple_var->count_indices != 0){
        printf("assignments with indices currently unsupported on avr_code_gen\n");
        exit(1);
    }

    struct TAC* t = makeTACCopy(
            a->var->simple_var->name,
            tacbuffer_last_dest(buffer)
    );

    tacbuffer_append(buffer, t, true);
}
