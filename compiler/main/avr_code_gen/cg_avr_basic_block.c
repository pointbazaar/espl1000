#include <stdbool.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "basic_block/basicblock.h"
#include "rat/rat.h"

#include "ibuffer/ibuffer.h"

#include "cg_avr_single_tac.h"
#include "cg_avr_basic_block.h"

static void allocate_registers(struct TACBuffer* b, struct RAT* rat);
static void allocate_registers_single_tac(struct TAC* t, struct RAT* rat);

void emit_asm_avr_basic_block(struct BasicBlock *block, struct Ctx* ctx, struct IBuffer* ibu) {

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

		emit_asm_avr_single_tac(rat, t, ctx, ibu);
    }

    //if(ctx->flags->debug)
        //rat_print(rat);

    rat_dtor(rat);

    //false/default branch gets emitted first,
    //because there is no label for it in a lot of cases
    //this way we can avoid an extra jump that's really 
    //not necessary.
    emit_asm_avr_basic_block(block->branch_2,  ctx, ibu);
    emit_asm_avr_basic_block(block->branch_1, ctx, ibu);
}

static void allocate_registers(struct TACBuffer* b, struct RAT* rat){
	
	for(size_t i=0;i < tacbuffer_count(b); i++){
        struct TAC* t = tacbuffer_get(b,i);
		allocate_registers_single_tac(t, rat);
    }
}

static void allocate_registers_single_tac(struct TAC* t, struct RAT* rat){

	//DEBUG
	//printf("allocate_registers_single_tac\n");

	bool iswide = true;

	switch(t->kind){

	case TAC_CONST_VALUE:
		iswide = ((uint16_t)t->const_value) > 255;
		rat_ensure_register(rat, t->dest, true, iswide);
		break;

	case TAC_BINARY_OP_IMMEDIATE:
		iswide = rat_is_wide(rat, t->dest);
		rat_ensure_register(rat, t->dest, true, iswide);
		break;

	case TAC_LOAD_LOCAL_ADDR:
		//address always needs 2 registers
		rat_ensure_register(rat, t->dest, true, true);
		break;

	case TAC_CALL:
		//TODO: make it depend on the width of the retured value
		rat_ensure_register(rat, t->dest, false, false);
		break;

	case TAC_COPY:
		iswide = rat_is_wide(rat, t->arg1);
		rat_ensure_register(rat, t->dest, false, iswide);
		break;

	case TAC_BINARY_OP:
		iswide = rat_is_wide(rat, t->arg1);
		rat_ensure_register(rat, t->dest, false, iswide);
		break;

	case TAC_UNARY_OP:
		iswide = rat_is_wide(rat, t->arg1);
		rat_ensure_register(rat, t->dest, false, iswide);
		break;

	case TAC_LOAD_LOCAL:
		//TODO: look at the LVST to see the width of the local var
		rat_ensure_register(rat, t->dest, false, true);
		break;

	case TAC_LOAD_CONST_ADDR:
		rat_ensure_register(rat, t->dest, false, true);
		break;

	case TAC_LOAD:
		//sadly we do not know what is all going to be added/subtracted
		//from what we load there, could be a pointer, so it must be wide
		rat_ensure_register(rat, t->dest, false, true);
		break;

	default: break;

	}
}
