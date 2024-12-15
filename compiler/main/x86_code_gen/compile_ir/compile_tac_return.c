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
	const size_t frame_size_bytes = lvst_stack_frame_size_x86(ctx_tables(ctx)->lvst);

	mov_const(rat_scratch_reg(rat), frame_size_bytes, c);
	add(SD_REG_RSP, rat_scratch_reg(rat), c);

	const int reg = rat_get_register(rat, tac->dest);

	mov_regs(SD_REG_RAX, reg, c);

	ret(c);
}
