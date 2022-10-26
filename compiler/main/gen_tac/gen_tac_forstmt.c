#include <stdio.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_forstmt(struct TACBuffer* buffer, struct ForStmt* f){

    //t1 = f->range->start
    //f->index_name = t1
    
    //L0:
    //t2 = f->range->end
    //t3 = f->index_name
    //t4 = t2
    //t4 = t4 >= t3
    //if-goto t4 L1
    //goto Lend:
    //L1:
    // block
    // t3 = f->index_name
    // t3++
    // goto L0
    //end:

    //f->index_name
    //f->range->start
    //f->range->end

    uint32_t l0 = make_label();
    uint32_t l1 = make_label();
    uint32_t lend = make_label();
    
    uint32_t t3 = make_temp();
    char t3str[10];
    sprintf(t3str, "t%d", t3);
    
    uint32_t t4 = make_temp();
    char t4str[10];
    sprintf(t4str, "t%d", t4);
    

    tac_expr(buffer, f->range->start);
    char* t1 = tacbuffer_last_dest(buffer);
    tacbuffer_append(buffer, makeTACCopy(f->index_name, t1));
    
    //L0:
    tacbuffer_append(buffer, makeTACLabel(l0));
    
    tac_expr(buffer, f->range->end);
    char* t2 = tacbuffer_last_dest(buffer);

    tacbuffer_append(buffer, makeTACCopy(t3str, f->index_name));
    tacbuffer_append(buffer, makeTACCopy(t4str, t2));

    tacbuffer_append(buffer, makeTACBinOp(t4str, TAC_OP_CMP_GE, t3str));

    struct TAC* t = makeTACIfGoto(t4str, l1);
    tacbuffer_append(buffer, t);

    tacbuffer_append(buffer, makeTACGoto(lend));

	//L1:
    tacbuffer_append(buffer, makeTACLabel(l1));

    tac_stmtblock(buffer, f->block);

    // t3++
    tacbuffer_append(buffer, makeTACCopy(t3str, f->index_name));
    tacbuffer_append(buffer, makeTACBinOpImmediate(t3str, TAC_OP_ADD, 1));
	tacbuffer_append(buffer, makeTACCopy(f->index_name, t3str));

    tacbuffer_append(buffer, makeTACGoto(l0));

    tacbuffer_append(buffer, makeTACLabel(lend));
}
