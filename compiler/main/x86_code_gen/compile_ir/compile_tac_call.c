#include <stdio.h>
#include <stdlib.h>

#include "ibuffer/ibuffer_x86.h"
#include "rat/rat_x86.h"
#include "tables/sst/sst.h"
#include "rat/rat.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "tac/tac.h"

#include "compile_tac.h"

void compile_tac_call_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx, char* current_function_name) {

	int reg_dest = rat_get_register(rat, tac_dest(tac));
	char* c = "TAC_CALL";

	struct SST* sst = ctx_tables(ctx)->sst;

	if (tac_arg1(tac) >= sst_size(sst)) {

		fprintf(stderr, "error: could not find function index %ld in sst\n", tac_arg1(tac));
		exit(1);
	}

	char* function_name = sst_at(sst, tac_arg1(tac))->name;

	const uint32_t arg_count = sst_args_count(sst, current_function_name);

	call(function_name, c);

	mov_regs(reg_dest, SD_REG_RAX, c);
}
