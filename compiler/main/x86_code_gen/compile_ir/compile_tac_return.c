#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "util/ctx.h"

void compile_tac_return_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	char* c = "TAC_RETURN";

	//destroy the stackframe
	//TODO: for now we assume the frame size is a multiple of 8 bytes.
	const size_t frame_size = lvst_stack_frame_size_avr(ctx_tables(ctx)->lvst) / 8;

	for (size_t k = 0; k < frame_size; k++) {

		pop(rat_scratch_reg(rat), "destroy frame");
	}

	int reg = rat_get_register(rat, tac->dest);

	mov_regs(0, reg, c);

	ret(c);
}
