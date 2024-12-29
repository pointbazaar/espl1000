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

static void save_arg_regs(struct IBuffer* ibu, uint32_t args_count) {

	char* c = "TAC_CALL (save arg regs)";
	for (int i = 0; i < args_count; i++) {
		enum SD_REGISTER reg = rat_param_reg_x86(i);
		push(reg, c);
	}
}

static void restore_arg_regs(struct IBuffer* ibu, uint32_t args_count) {

	char* c = "TAC_CALL (save arg regs)";
	for (int i = args_count - 1; i >= 0; i--) {
		enum SD_REGISTER reg = rat_param_reg_x86(i);
		pop(reg, c);
	}
}

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
	// save our parameter registers
	save_arg_regs(ibu, arg_count);

	call(function_name, c);

	// restore our parameter registers
	restore_arg_regs(ibu, arg_count);

	mov_regs(reg_dest, SD_REG_RAX, c);
}
