#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "ibuffer/ibuffer_x86.h"
#include "rat/rat_x86.h"
#include "tables/sst/sst.h"
#include "rat/rat.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "tac/tac.h"

#include "compile_tac.h"

#include "x86_code_gen/syscalls/syscalls.h"

bool compile_tac_call_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx, char* current_function_name) {

	(void)current_function_name;

	int reg_dest = rat_get_register(rat, tac_dest(tac));
	char* c = "TAC_CALL";

	struct SST* sst = ctx_tables(ctx)->sst;

	if (tac_arg1(tac) >= sst_size(sst)) {

		fprintf(stderr, "error: could not find function index %ld in sst\n", tac_arg1(tac));
		return false;
	}

	const struct SSTLine* line = sst_at(sst, tac_arg1(tac));

	char* function_name = line->name;

	if (line->is_syscall) {
		mov_const(SD_REG_RAX, syscall_number(line->name), c);
		x86_syscall(c);
	} else {
		call(function_name, c);
	}

	mov_regs(reg_dest, SD_REG_RAX, c);

	return true;
}
