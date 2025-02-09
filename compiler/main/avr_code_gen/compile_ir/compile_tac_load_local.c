#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "cli/flags/flags.h"
#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_local(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	const int reg_dest = rat_get_register(rat, tac_dest(tac));

	struct LVSTLine* line = lvst_at(ctx_tables(ctx)->lvst, tac_arg1(tac));
	char* name = line->name;

	const uint32_t width = lvst_sizeof_type(line->type, flags_x86(ctx_flags(ctx)));

	if (width == 2) {
		assert(rat_is_wide(rat, tac_dest(tac)));
	}

	assert(width >= 1);
	assert(width <= 2);

	const uint16_t offset = lvst_stack_frame_offset_avr(ctx_tables(ctx)->lvst, name);

	if (offset == 0) {
		//ld reg_dest, Y
		ldY(reg_dest, "TAC_LOAD_LOCAL");
	} else {
		lddY(reg_dest, offset, "TAC_LOAD_LOCAL");
		//ldd reg_dest, Y+offset
	}

	if (width == 2) {
		lddY(reg_dest + 1, offset + 1, "TAC_LOAD_LOCAL");
	}
}
