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

void compile_tac_icall_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx, char* current_function_name) {

	(void)current_function_name;
	(void)ctx;

	const int reg_dest = rat_get_register(rat, tac_dest(tac));
	char* c = "TAC_ICALL";

	const int reg_arg1 = rat_get_register(rat, tac_arg1(tac));

	icall(reg_arg1, c);

	mov_regs(reg_dest, SD_REG_RAX, c);
}
