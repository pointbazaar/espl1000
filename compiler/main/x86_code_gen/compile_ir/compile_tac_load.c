#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_LOAD";

	//tdest = [targ1]

	const int reg_dest = rat_get_register(rat, tac->dest);

	const int reg_src = rat_get_register(rat, tac->arg1);

	mov(XL, reg_src, c);

	ldi(XH, 0, c);

	ldX(reg_dest, c);
}
