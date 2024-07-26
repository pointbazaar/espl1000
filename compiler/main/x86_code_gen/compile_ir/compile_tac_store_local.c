#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"

#include "tac/tac.h"

#include "util/ctx.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_store_local_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	char* name = lvst_at(ctx_tables(ctx)->lvst, tac->dest)->name;

	struct LVST* lvst = ctx_tables(ctx)->lvst;

	size_t offset = lvst_stack_frame_offset_avr(lvst, name);

	const int reg = rat_get_register(rat, tac->arg1);

	const uint32_t var_width = lvst_sizeof_var(lvst, name);

	if (var_width == 0) {
		fprintf(stderr, "%s could not determine width of local variable %s\n", __FILE__, name);
		exit(1);
	}

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);

	if (offset == 0) {
		stY(reg, "TAC_STORE_LOCAL");
		//st Y, reg; writeback locals
	} else {
		stdY(offset, reg, "TAC_STORE_LOCAL");
		//std Y+offset, reg; writeback locals
	}

	// in case the value to store is only 8 bit,
	// the upper 8 bit of the variable need
	// to be cleared for subsequent loads.
	ldi(RAT_SCRATCH_REG, 0, "TAC_STORE_LOCAL");
	stdY(offset + 1, RAT_SCRATCH_REG, "TAC_STORE_LOCAL");
}
