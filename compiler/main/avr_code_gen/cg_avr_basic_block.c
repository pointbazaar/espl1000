#include <stdbool.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "basic_block/basicblock.h"
#include "rat/rat.h"

#include "cg_avr_single_tac.h"
#include "cg_avr_basic_block.h"

static void allocate_registers(struct TACBuffer* b, struct RAT* rat);
static void allocate_registers_single_tac(struct TAC* t, struct RAT* rat);

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
    allocate_registers(block->buffer, rat);

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

static void allocate_registers(struct TACBuffer* b, struct RAT* rat){
	
	for(size_t i=0;i < tacbuffer_count(b); i++){
        struct TAC* t = tacbuffer_get(b,i);
		allocate_registers_single_tac(t, rat);
    }
}

static void allocate_registers_single_tac(struct TAC* t, struct RAT* rat){
	
	switch(t->kind){
		case TAC_CONST_VALUE:
		case TAC_BINARY_OP_IMMEDIATE:
			rat_ensure_register(rat, t->dest, true, t->wide); 
			break;
		case TAC_CALL:
		case TAC_COPY:
		case TAC_BINARY_OP:
		case TAC_UNARY_OP:
		case TAC_LOAD_LOCAL:
		case TAC_LOAD_CONST_ADDR:
		case TAC_LOAD:
			rat_ensure_register(rat, t->dest, false, t->wide); 
			break;
		default: break;
	}
}
