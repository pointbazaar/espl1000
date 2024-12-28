#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"

#include "tac/tac.h"

#include "util/ctx.h"

#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_store_local_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	char* c = "TAC_STORE_LOCAL";
	char* name = lvst_at(ctx_tables(ctx)->lvst, tac_dest(tac))->name;

	struct LVST* lvst = ctx_tables(ctx)->lvst;

	const ssize_t offset = lvst_stack_frame_offset_x86(lvst, name);

	const int reg = rat_get_register(rat, tac_arg1(tac));

	const uint32_t var_width = lvst_sizeof_var(lvst, name);

	if (var_width == 0) {
		fprintf(stderr, "%s could not determine width of local variable %s\n", __FILE__, name);
		exit(1);
	}

	const int rscratch = rat_scratch_reg(rat);

	mov_const(rscratch, -offset, c);
	add(rscratch, rat_base_ptr(rat), c);
	mov_store(rscratch, reg, c);
}
