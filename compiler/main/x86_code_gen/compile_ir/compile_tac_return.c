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

static void callee_restore_registers(struct RAT* rat, struct IBuffer* ibu) {

	char* c = "TAC_RETURN (restore callee saved regs)";
	for (enum SD_REGISTER reg = SD_REG_END_X86 - 1; reg >= SD_REG_RBX; reg--) {
		if (rat_callee_must_save(rat, reg)) {
			pop(reg, c);
		}
	}
}

void compile_tac_return_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	char* c = "TAC_RETURN";

	//destroy the stackframe
	const size_t frame_size_bytes = lvst_stack_frame_size_x86(ctx_tables(ctx)->lvst);

	if (frame_size_bytes > 0) {

		mov_const(rat_scratch_reg(rat), frame_size_bytes, c);
		add(SD_REG_RSP, rat_scratch_reg(rat), c);
	}

	const int reg = rat_get_register(rat, tac_dest(tac));

	mov_regs(SD_REG_RAX, reg, c);

	callee_restore_registers(rat, ibu);

	ret(c);
}
