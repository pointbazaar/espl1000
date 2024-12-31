#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"
#include "rat/rat_x86.h"
#include "tac/tac.h"
#include "util/ctx.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "x86_code_gen/compile_ir/compile_tac.h"

static void case_arg(struct RAT* rat, struct TAC* tac, struct LVST* lvst, struct IBuffer* ibu, char* name, int reg_dest) {
	char* c = "TAC_LOAD_LOCAL (arg)";

	const uint32_t param_index = lvst_arg_index(lvst, name);
	const uint32_t param_reg = rat_param_reg_x86(param_index);
	const uint32_t offset = lvst_stack_frame_offset_x86(lvst, name);
	const uint8_t width = lvst_sizeof_var(lvst, name, true);

	mov_const(rat_scratch_reg(rat), -offset, c);
	add(rat_scratch_reg(rat), rat_base_ptr(rat), c);

	// clear the upper bits
	xor(reg_dest, reg_dest, c);
	mov_load_width(reg_dest, rat_scratch_reg(rat), width, c);
}

static void case_local_var(struct RAT* rat, struct TAC* tac, struct LVST* lvst, struct IBuffer* ibu, char* name, int reg_dest) {

	char* c = "TAC_LOAD_LOCAL (local var)";

	const ssize_t offset = lvst_stack_frame_offset_x86(lvst, name);

	const uint32_t nbytes = lvst_sizeof_var(lvst, name, true);
	assert(nbytes > 0);

	// stack frame is pointed to by rbp
	uint32_t rscratch = rat_scratch_reg(rat);
	mov_const(rscratch, -offset, c);
	add(rscratch, SD_REG_RBP, c);

	mov_load_width(reg_dest, rscratch, nbytes, c);
}

void compile_tac_load_local_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	const int reg_dest = rat_get_register(rat, tac_dest(tac));
	struct LVST* lvst = ctx_tables(ctx)->lvst;

	char* name = lvst_at(lvst, tac_arg1(tac))->name;

	struct LVSTLine* line = lvst_get(lvst, name);
	assert(line != NULL);

	if (line->is_arg) {
		case_arg(rat, tac, lvst, ibu, name, reg_dest);
	} else {
		case_local_var(rat, tac, lvst, ibu, name, reg_dest);
	}
}
