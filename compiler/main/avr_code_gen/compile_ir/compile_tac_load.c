#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_load(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_LOAD";

	//tdest = [targ1]

	const int reg_dest = rat_get_register(rat, tac->dest);

	const int reg_src = rat_get_register(rat, tac->arg1);

	mov(XL, reg_src, c);

	if (rat_is_wide(rat, tac->arg1))
		mov(XH, reg_src + 1, c);
	else
		ldi(XH, 0, c);

	// TODO: make a 'wide' property in struct TAC
	// to avoid encoding this in different places
	const bool tac_says_wide = tac->const_value == 2;

	if (tac_says_wide) {

		// what if we want to load a 16 bit value?
		ldXpostInc(reg_dest, c);
		ldX(reg_dest + 1, c);
	} else {
		ldX(reg_dest, c);

		if (rat_is_wide(rat, tac->dest)) {
			clr(reg_dest + 1, c);
		}
	}
}
