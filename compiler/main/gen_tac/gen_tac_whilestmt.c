#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_whilestmt(struct TACBuffer* buffer, struct WhileStmt* w, struct Ctx* ctx){

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
    
    ctx_enter_loop(ctx, l0, lend);

    tacbuffer_append(buffer, makeTACLabel(l0));

    tac_expr(buffer, w->condition);

    struct TAC* t = makeTACIfGoto(tacbuffer_last_dest(buffer), l1);
    tacbuffer_append(buffer, t);

    tacbuffer_append(buffer, makeTACGoto(lend));

    tacbuffer_append(buffer, makeTACLabel(l1));

    tac_stmtblock(buffer, w->block, ctx);

    tacbuffer_append(buffer, makeTACGoto(l0));

    tacbuffer_append(buffer, makeTACLabel(lend));
    
    ctx_exit_loop(ctx);
}
