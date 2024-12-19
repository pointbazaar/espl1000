#include <stdio.h>
#include <stdlib.h>

#include "ibuffer/ibuffer_x86.h"
#include "tables/sst/sst.h"
#include "rat/rat.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "tac/tac.h"

#include "compile_tac.h"

void compile_tac_call_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx) {

	int reg_dest = rat_get_register(rat, tac->dest);
	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);
	char* c = "TAC_CALL";

	char* function_name = NULL;

	if (tac->arg1 < sst_size(ctx_tables(ctx)->sst)) {
		function_name = sst_at(ctx_tables(ctx)->sst, tac->arg1)->name;
	} else {
		fprintf(stderr, "error: could not find function index %ld in sst\n", tac->arg1);
		exit(1);
	}

	call(function_name, c);

	mov_regs(reg_dest, SD_REG_RAX, c);

	struct SST* sst = ctx_tables(ctx)->sst;

	//pop the PARAMS off the stack
	char* name = sst_at(sst, tac->arg1)->name;

	uint32_t size = sst_args_size_x86(sst, name);

	mov_const(rat_scratch_reg(rat), size, c);
	add(SD_REG_RSP, rat_scratch_reg(rat), c);
}
