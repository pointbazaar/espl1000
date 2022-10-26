
#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

static void tac_ifstmt_1_block(struct TACBuffer* buffer, struct IfStmt* s, struct Ctx* ctx);
static void tac_ifstmt_2_block(struct TACBuffer* buffer, struct IfStmt* s, struct Ctx* ctx);

void tac_ifstmt(struct TACBuffer* buffer, struct IfStmt* s, struct Ctx* ctx){

    if(s->else_block == NULL)
        tac_ifstmt_1_block(buffer, s, ctx);
    else
        tac_ifstmt_2_block(buffer, s, ctx);
}

static void tac_ifstmt_1_block(struct TACBuffer* buffer, struct IfStmt* s, struct Ctx* ctx){
    
    //if-goto expr ltrue
    //goto lend
    // ltrue:
    //  if block
    //lend:

    uint32_t lend = make_label();
    uint32_t ltrue = make_label();

    tac_expr(buffer, s->condition);

    //negate the condition
    //struct TAC* t_neg_cond = makeTACUnaryOp(tacbuffer_last_dest(buffer), tacbuffer_last_dest(buffer), TAC_OP_UNARY_NOT);
    //tacbuffer_append(buffer, t_neg_cond);

    struct TAC* t_if_goto_end = makeTACIfGoto(tacbuffer_last_dest(buffer), ltrue);
    tacbuffer_append(buffer, t_if_goto_end);
    
    tacbuffer_append(buffer, makeTACGoto(lend));

	tacbuffer_append(buffer, makeTACLabel(ltrue));
    tac_stmtblock(buffer, s->block, ctx);

    tacbuffer_append(buffer, makeTACLabel(lend));
}

static void tac_ifstmt_2_block(struct TACBuffer* buffer, struct IfStmt* s, struct Ctx* ctx){
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
    tacbuffer_append(buffer, t);

    tacbuffer_append(buffer, makeTACGoto(l2));

    tacbuffer_append(buffer, makeTACLabel(l1));

    tac_stmtblock(buffer, s->block, ctx);

    tacbuffer_append(buffer, makeTACGoto(lend));

    tacbuffer_append(buffer, makeTACLabel(l2));

    tac_stmtblock(buffer, s->else_block, ctx);

    tacbuffer_append(buffer, makeTACLabel(lend));
}
