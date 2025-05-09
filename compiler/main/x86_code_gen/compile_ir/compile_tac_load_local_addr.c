#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ibuffer/ibuffer_x86.h"
#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"

#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_local_addr_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	char* c = "TAC_LOAD_LOCAL_ADDR";

	char* name = lvst_at(ctx_tables(ctx)->lvst, tac_arg1(tac))->name;

	// In x86-64, arguments are passed in registers, but we do save
	// them into the stack frame when the stack frame is setup.
	// So each variable, no matter if arg or local, should have an address.

	const int rdest = rat_get_register(rat, tac_dest(tac));

	const ssize_t offset = lvst_stack_frame_offset_x86(ctx_tables(ctx)->lvst, name);

	mov_const(rdest, -offset, c);

	add(rdest, rat_base_ptr(rat), c);
}
