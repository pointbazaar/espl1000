#include <stdbool.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "basic_block/basicblock.h"
#include "rat/rat.h"

#include "cg_avr_single_tac.h"
#include "cg_avr_basic_block.h"


void emit_asm_avr_basic_block(struct BasicBlock *block, struct Ctx* ctx, FILE *fout) {

    if(block == NULL || block->visited_emit_asm)
        return;

    block->visited_emit_asm = true;

    //create register allocation table for the basic block.
    struct RAT* rat = rat_ctor(ctx_tables(ctx));

    //simplest naive approach (first iteration):
    //simply get a new register for each temporary
    //the mapping tx -> ry can be saved in an array
    //TODO: use better approach

    for(size_t i=0;i < tacbuffer_count(block->buffer); i++){
        struct TAC* t = tacbuffer_get(block->buffer,i);

		emit_asm_avr_single_tac(rat, t, ctx, fout);
    }

    //if(ctx->flags->debug)
        //rat_print(rat);

    rat_dtor(rat);

    //false/default branch gets emitted first,
    //because there is no label for it in a lot of cases
    //this way we can avoid an extra jump that's really 
    //not necessary.
    emit_asm_avr_basic_block(block->branch_2,  ctx, fout);
    emit_asm_avr_basic_block(block->branch_1, ctx, fout);
}
