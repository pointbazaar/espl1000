#include <stdbool.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"

#include "flags.h"
#include "tac.h"
#include "tacbuffer.h"
#include "basicblock.h"
#include "rat.h"

#include "cg_avr_single_tac.h"
#include "cg_avr_basic_block.h"

void emit_asm_avr_basic_block(struct BasicBlock *block, struct Ctx* ctx, FILE *fout) {

    if(block == NULL || block->visited_emit_asm)
        return;

    block->visited_emit_asm = true;

    //create register allocation table for the basic block.
    struct RAT* rat = rat_ctor(ctx->tables);

    //simplest naive approach (first iteration):
    //simply get a new register for each temporary
    //the mapping tx -> ry can be saved in an array
    //TODO: use better approach

    for(size_t i=0;i < block->buffer->count; i++){
        struct TAC* t = block->buffer->buffer[i];
        emit_asm_avr_single_tac(rat, t, ctx, fout);
    }

    if(ctx->flags->debug)
        rat_print(rat);

    //TODO: store all locals into the stack frame
    //at the end of a basic block

    emit_asm_avr_basic_block(block->branch_1,  ctx, fout);
    emit_asm_avr_basic_block(block->branch_2, ctx, fout);
}