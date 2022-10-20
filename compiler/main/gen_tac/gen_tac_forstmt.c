#include <stdio.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_forstmt(struct TACBuffer* buffer, struct ForStmt* f){

    //t1 = f->range->start
    //t2 = f->range->end
    //L0:
    //t3 = t1
    //t3 = t3 <= t2
    //if-goto t3 L1
    //goto Lend:
    //L1:
    // block
    // t3++
    // goto L0
    //end:

    //f->index_name
    //f->range->start
    //f->range->end

    uint32_t l0 = make_label();
    uint32_t l1 = make_label();
    uint32_t lend = make_label();

    tac_expr(buffer, f->range->start);
    char* t1 = tacbuffer_last_dest(buffer);
    tac_expr(buffer, f->range->end);
    char* t2 = tacbuffer_last_dest(buffer);

    tacbuffer_append(buffer, makeTACLabel(l0));

    uint32_t t3 = make_temp();
    char t3c[10];
    sprintf(t3c, "t%d", t3);

    tacbuffer_append(buffer, makeTACCopy(t3c, t1));
    
    char t3str[10];
    sprintf(t3str, "t%d", t3);

    tacbuffer_append(buffer, makeTACBinOp(t2, TAC_OP_CMP_GE, t3str));

    struct TAC* t = makeTACIfGoto(t3c, l1);
    tacbuffer_append(buffer, t);

    tacbuffer_append(buffer, makeTACGoto(lend));

    tacbuffer_append(buffer, makeTACLabel(l1));

    tac_stmtblock(buffer, f->block);

    // t3++
    tacbuffer_append(buffer, makeTACBinOpImmediate(t3c, TAC_OP_ADD, 1));

    tacbuffer_append(buffer, makeTACGoto(l0));

    tacbuffer_append(buffer, makeTACLabel(lend));
}
