#include <stdbool.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "basic_block/basicblock.h"
#include "rat/rat.h"

#include "ibuffer/ibuffer_x86.h"

#include "cli/flags/flags.h"

#include "cg_x86_single_tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"
#include "cg_x86_basic_block.h"

void allocate_registers(struct TACBuffer* b, struct RAT* rat, struct ST* st);
static void allocate_registers_single_tac(struct TAC* t, struct RAT* rat, struct ST* st);

void emit_asm_x86_basic_block(struct BasicBlock* block, struct Ctx* ctx, struct IBuffer* ibu, struct RAT* rat) {

	if (block == NULL) {
		return;
	}
	if (block->visited_emit_asm) {
		return;
	}

	block->visited_emit_asm = true;

	if (flags_debug(ctx_flags(ctx)))
		rat_print(rat);

	for (size_t i = 0; i < tacbuffer_count(block->buffer); i++) {
		struct TAC* t = tacbuffer_get(block->buffer, i);

		emit_asm_x86_single_tac(rat, t, ctx, ibu);
	}

	if (flags_debug(ctx_flags(ctx)))
		rat_print(rat);

	//false/default branch gets emitted first,
	//because there is no label for it in a lot of cases
	//this way we can avoid an extra jump that's really
	//not necessary.
	emit_asm_x86_basic_block(block->branch_2, ctx, ibu, rat);
	emit_asm_x86_basic_block(block->branch_1, ctx, ibu, rat);
}
