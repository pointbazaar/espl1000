#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_whilestmt(struct TACBuffer* buffer, struct WhileStmt* w){

    //L0:
    //t1 = expr
    //if-goto t1 L1
    //goto Lend:
    //L1:
    // block
    // goto L0
    //end:

    uint32_t l0 = make_label();
    uint32_t l1 = make_label();
    uint32_t lend = make_label();

    tacbuffer_append(buffer, makeTACLabel(l0), true);

    tac_expr(buffer, w->condition);

    struct TAC* t = makeTACIfGoto(buffer->buffer[buffer->count-1]->dest, l1);
    tacbuffer_append(buffer, t, true);

    tacbuffer_append(buffer, makeTACGoto(lend), true);

    tacbuffer_append(buffer, makeTACLabel(l1), true);

    tac_stmtblock(buffer, w->block);

    tacbuffer_append(buffer, makeTACGoto(l0), true);

    tacbuffer_append(buffer, makeTACLabel(lend), true);
}