
#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

static void tac_ifstmt_1_block(struct TACBuffer* buffer, struct IfStmt* s);
static void tac_ifstmt_2_block(struct TACBuffer* buffer, struct IfStmt* s);

void tac_ifstmt(struct TACBuffer* buffer, struct IfStmt* s){

    if(s->else_block == NULL)
        tac_ifstmt_1_block(buffer, s);
    else
        tac_ifstmt_2_block(buffer, s);
}

static void tac_ifstmt_1_block(struct TACBuffer* buffer, struct IfStmt* s){
    //t_neg_cond = expr
    //t_neg_cond = !t_neg_cond
    //if-goto t_neg_cond end
    // if block
    //end:

    uint32_t lend = make_label();

    tac_expr(buffer, s->condition);

    //negate the condition
    struct TAC* t_neg_cond = makeTACUnaryOp(tacbuffer_last_dest(buffer), tacbuffer_last_dest(buffer), TAC_OP_UNARY_NOT);
    tacbuffer_append(buffer, t_neg_cond, true);

    struct TAC* t_if_goto_end = makeTACIfGoto(tacbuffer_last_dest(buffer), lend);
    tacbuffer_append(buffer, t_if_goto_end, true);

    tac_stmtblock(buffer, s->block);

    tacbuffer_append(buffer, makeTACLabel(lend), true);
}

static void tac_ifstmt_2_block(struct TACBuffer* buffer, struct IfStmt* s){
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

    struct TAC* t = makeTACIfGoto(tacbuffer_last_dest(buffer), l1);
    tacbuffer_append(buffer, t, true);

    tacbuffer_append(buffer, makeTACGoto(l2), true);

    tacbuffer_append(buffer, makeTACLabel(l1), true);

    tac_stmtblock(buffer, s->block);

    tacbuffer_append(buffer, makeTACGoto(lend), true);

    tacbuffer_append(buffer, makeTACLabel(l2), true);

    tac_stmtblock(buffer, s->else_block);

    tacbuffer_append(buffer, makeTACLabel(lend), true);
}
