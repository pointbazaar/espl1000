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

void emit_asm_x86_basic_block(struct BasicBlock* block, struct Ctx* ctx, struct IBuffer* ibu, struct RAT* rat, char* current_function_name) {

	if (block == NULL) {
		return;
	}
	if (block->visited_emit_asm) {
		return;
	}

	block->visited_emit_asm = true;

	for (size_t i = 0; i < tacbuffer_count(block->buffer); i++) {
		struct TAC* t = tacbuffer_get(block->buffer, i);

		emit_asm_x86_single_tac(rat, t, ctx, ibu, current_function_name);
	}

	//false/default branch gets emitted first,
	//because there is no label for it in a lot of cases
	//this way we can avoid an extra jump that's really
	//not necessary.
	emit_asm_x86_basic_block(block->branch_2, ctx, ibu, rat, current_function_name);
	emit_asm_x86_basic_block(block->branch_1, ctx, ibu, rat, current_function_name);
}
