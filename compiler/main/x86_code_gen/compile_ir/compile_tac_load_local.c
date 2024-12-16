#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_local_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	char* c = "TAC_LOAD_LOCAL";
	const int reg_dest = rat_get_register(rat, tac->dest);

	char* name = lvst_at(ctx_tables(ctx)->lvst, tac->arg1)->name;

	const ssize_t offset = lvst_stack_frame_offset_x86(ctx_tables(ctx)->lvst, name);

	// stack frame is pointed to by rbp
	uint32_t rscratch = rat_scratch_reg(rat);
	mov_const(rscratch, -offset, c);
	add(rscratch, SD_REG_RBP, c);
	mov_load(reg_dest, rscratch, c);
}
