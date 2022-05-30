
#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

void tac_ifstmt(struct TACBuffer* buffer, struct IfStmt* s){

    //t1 = expr
    //if-goto t1 L1
    //goto L2:
    //L1:
    // if block
    // goto end
    //L2:
    // else block
    //end:

    uint32_t l1 = make_label();
    uint32_t l2 = make_label();
    uint32_t lend = make_label();

    tac_expr(buffer, s->condition);

    struct TAC* t = makeTACIfGoto(buffer->buffer[buffer->count-1]->dest, l1);
    tacbuffer_append(buffer, t, true);

    tacbuffer_append(buffer, makeTACGoto(l2), true);

    tacbuffer_append(buffer, makeTACLabel(l1), true);

    tac_stmtblock(buffer, s->block);

    tacbuffer_append(buffer, makeTACGoto(lend), true);

    tacbuffer_append(buffer, makeTACLabel(l2), true);

    if(s->else_block != NULL)
        tac_stmtblock(buffer, s->else_block);

    tacbuffer_append(buffer, makeTACLabel(lend), true);
}