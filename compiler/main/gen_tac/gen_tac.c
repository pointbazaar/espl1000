#include "gen_tac.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "util/ctx.h"
#include "tac/tacbuffer.h"

int int_value_from_const(struct ConstValue* cv){

    switch (cv->kind) {
        case 1: return (int) cv->ptr.m1_bool_const;
        case 2: return (int) cv->ptr.m2_int_const;
        case 3: return (int) cv->ptr.m3_char_const;
        case 5: return (int) cv->ptr.m5_hex_const;
        case 6: return (int) cv->ptr.m5_hex_const;
    }
    return -1;
}

void tac_method(struct TACBuffer* buffer, struct Method* m, struct Ctx* ctx){

	const uint32_t index = sst_index_of(ctx_tables(ctx)->sst, m->decl->name);

    //first the label of the function
    tacbuffer_append(buffer, makeTACLabelFunction(index));
    
    //setup the stack frame
    tacbuffer_append(buffer, makeTACSetupStackframe(lvst_stack_frame_size_avr(ctx_tables(ctx)->lvst)));

    tac_stmtblock(buffer, m->block, ctx);
}

void tac_stmtblock(struct TACBuffer* buffer, struct StmtBlock* block, struct Ctx* ctx){
    for(size_t k = 0; k < block->count; k++)
        tac_stmt(buffer, block->stmts[k], ctx);
}

