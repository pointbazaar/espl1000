#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_retstmt(struct TACBuffer* buffer, struct RetStmt* r){

    if(r->return_value != NULL){
        tac_expr(buffer, r->return_value);
    }

    struct TAC* t = makeTAC();
    t->kind = TAC_RETURN;
    strcpy(t->arg1, buffer->buffer[buffer->count-1]->dest);

    tacbuffer_append(buffer, t, true);
}