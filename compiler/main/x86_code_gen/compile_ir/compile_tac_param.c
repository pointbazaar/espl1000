#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"

#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_param_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const char* c = "TAC_PARAM";

	const int reg_dest = rat_get_register(rat, tac_dest(tac));

	push(reg_dest, c);
}
