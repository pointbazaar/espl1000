#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_load(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	//tdest = [targ1]

	const int reg_dest = rat_get_register(rat, tac->dest);

	const int reg_src = rat_get_register(rat, tac->arg1);

	mov(XL, reg_src);

	if(rat_is_wide(rat, tac->arg1))
		mov(XH, reg_src+1);
	else
		ldi(XH, 0, "TAC_LOAD");

	ldX(reg_dest, "TAC_LOAD"); //ld reg_dest, X

	if(rat_is_wide(rat, tac->dest)){
		//destination is wide, we have to zero out the high reg
		clr(reg_dest+1, "TAC_LOAD");
	}
}
