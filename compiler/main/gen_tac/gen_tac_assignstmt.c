#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

void tac_assignstmt(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx){

    tac_expr(buffer, a->expr, ctx);

    if(a->var->member_access != NULL){
        printf("member access assignments currently unsupported on avr_code_gen\n");
        exit(1);
    }

    if(a->var->simple_var->count_indices != 0){
        printf("assignments with indices currently unsupported on avr_code_gen\n");
        exit(1);
    }
    
    const uint32_t local_index = lvst_index_of(ctx_tables(ctx)->lvst, a->var->simple_var->name);
    
    struct TAC* t = makeTACStoreLocal(
			local_index,
            tacbuffer_last_dest(buffer)
	);

    tacbuffer_append(buffer, t);
}
