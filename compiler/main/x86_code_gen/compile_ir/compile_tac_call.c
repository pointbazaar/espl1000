#include <stdio.h>

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "tac/tac.h"

#include "compile_tac.h"

void compile_tac_call_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx) {

	int reg_dest = rat_get_register(rat, tac->dest);
	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);

	//in case of tests, where SST may not be filled correctly
	char* function_name = "main";

	if (tac->arg1 < sst_size(ctx_tables(ctx)->sst)) {
		function_name = sst_at(ctx_tables(ctx)->sst, tac->arg1)->name;
	}

	call(function_name, "TAC_CALL");

	if (reg_dest != 0)
		mov(reg_dest, 0, "TAC_CALL");

	struct SST* sst = ctx_tables(ctx)->sst;

	//for the case of tests on raw TACBuffers
	if (sst_size(sst) == 0)
		return;

	//pop the PARAMS off the stack
	char* name = sst_at(sst, tac->arg1)->name;

	uint32_t size = sst_args_size_avr(sst, name);

	for (uint32_t i = 0; i < size; i++) {
		pop(RAT_SCRATCH_REG, "remove call params");
	}
}
