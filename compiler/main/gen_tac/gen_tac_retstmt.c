#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_retstmt(struct TACBuffer* buffer, struct RetStmt* r){

    if(r->return_value != NULL){
        tac_expr(buffer, r->return_value);
    }

    struct TAC* t = makeTACReturn(tacbuffer_last_dest(buffer));

    tacbuffer_append(buffer, t);
}
