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

void compile_tac_load_function_ptr_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	char* c = "TAC_LOAD_FUNCTION_PTR";

	struct SST* sst = ctx_tables(ctx)->sst;

	char* name = sst_at(sst, tac_arg1(tac))->name;

	const int rdest = rat_get_register(rat, tac_dest(tac));

	mov_const_symbol(rdest, name, c);
}
