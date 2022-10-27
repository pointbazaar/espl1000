#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_forstmt(struct TACBuffer* buffer, struct ForStmt* f, struct Ctx* ctx){

    //t1 = f->range->start
    //f->index_name = t1
    
    //L0:
    //t2 = f->range->end
    //t3 = f->index_name
    //if-cmp-goto t2 >= t3 L1    
    
    //goto Lend:
    
    //L1:
    // block
    // t3 = f->index_name
    // t3++
    // goto L0
    //Lend:

    //f->index_name
    //f->range->start
    //f->range->end

    uint32_t l0 = make_label();
    uint32_t l1 = make_label();
    uint32_t lend = make_label();
    
    ctx_enter_loop(ctx, l0, lend);
    
    uint32_t t3 = make_temp();
    char t3str[10];
    sprintf(t3str, "t%d", t3);

    tac_expr(buffer, f->range->start);
    char* t1 = tacbuffer_last_dest(buffer);
    tacbuffer_append(buffer, makeTACStoreLocal(f->index_name, atoi(t1+1)));
    
    //L0:
    tacbuffer_append(buffer, makeTACLabel(l0));
    
    tac_expr(buffer, f->range->end);
    char* t2 = tacbuffer_last_dest(buffer);

    tacbuffer_append(buffer, makeTACLoadLocal(t3, f->index_name));
    
    tacbuffer_append(buffer, makeTACIfCMPGoto(atoi(t2+1), TAC_OP_CMP_GE, t3, l1));

    tacbuffer_append(buffer, makeTACGoto(lend));

	//L1:
    tacbuffer_append(buffer, makeTACLabel(l1));

    tac_stmtblock(buffer, f->block, ctx);

    // t3++
    tacbuffer_append(buffer, makeTACLoadLocal(t3, f->index_name));
    tacbuffer_append(buffer, makeTACBinOpImmediate(t3str, TAC_OP_ADD, 1));
	tacbuffer_append(buffer, makeTACStoreLocal(f->index_name, t3));

    tacbuffer_append(buffer, makeTACGoto(l0));

	//Lend:
    tacbuffer_append(buffer, makeTACLabel(lend));
    
    ctx_exit_loop(ctx);
}
