#include <stdbool.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"

#include "flags.h"

#include "avr_code_gen/tac/tac.h"
#include "avr_code_gen/tacbuffer/tacbuffer.h"
#include "avr_code_gen/basic_block/basicblock.h"
#include "rat/rat.h"

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

        if(i == block->buffer->count - 1){
            if(t->kind == TAC_GOTO || t->kind == TAC_IF_GOTO
            || t->kind == TAC_RETURN || t->kind == TAC_CALL){
                //TODO
                //store all locals that have been written, into the stack frame before
                //we leave this basic block

                for(int k=0;k < rat->capacity; k++) {

                    if(!rat->is_occupied[k]) continue;

                    char* var_name = rat->occupant[k];

                    if(lvst_contains(ctx->tables->lvst, var_name)){
                        size_t offset = lvst_stack_frame_offset_avr(ctx->tables->lvst, var_name);

                        if(offset == 0){
                            fprintf(fout, "st Y, r%d\n", k);
                        }else {
                            fprintf(fout, "std Y+%zu, r%d\n", offset, k);
                        }
                    }
                }
            }
        }

        emit_asm_avr_single_tac(rat, t, ctx, fout);
    }

    //if(ctx->flags->debug)
        //rat_print(rat);

    rat_dtor(rat);

    emit_asm_avr_basic_block(block->branch_1,  ctx, fout);
    emit_asm_avr_basic_block(block->branch_2, ctx, fout);
}