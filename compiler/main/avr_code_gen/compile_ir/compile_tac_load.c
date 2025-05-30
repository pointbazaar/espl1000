#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_load(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_LOAD";

	//tdest = [targ1]

	const int reg_dest = rat_get_register(rat, tac_dest(tac));

	const int reg_src = rat_get_register(rat, tac_arg1(tac));

	avr_mov(XL, reg_src, c);

	if (rat_is_wide(rat, tac_arg1(tac)))
		avr_mov(XH, reg_src + 1, c);
	else
		avr_ldi(XH, 0, c);

	const uint8_t width = tac_load_store_width(tac);

	assert(width <= 2);
	assert(width >= 1);

	if (width == 2) {

		// what if we want to load a 16 bit value?
		avr_ldXpostInc(reg_dest, c);
		avr_ldX(reg_dest + 1, c);
	} else {
		avr_ldX(reg_dest, c);

		if (rat_is_wide(rat, tac_dest(tac))) {
			avr_clr(reg_dest + 1, c);
		}
	}
}
