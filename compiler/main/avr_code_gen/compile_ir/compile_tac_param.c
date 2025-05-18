#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_param(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_PARAM";

	const int reg_dest = rat_get_register(rat, tac_dest(tac));

	assert(tac_const_value(tac) % 8 == 0);
	assert(tac_const_value(tac) <= 64);

	if (tac_const_value(tac) == 16) {
		// this means it's a 16 bit parameter
		// so we must push twice

		//TODO: ensure that the temporary has the correct width in RAT
		//assert(rat_is_wide(rat, tac->dest));

		if (rat_is_wide(rat, tac_dest(tac))) {
			avr_push(reg_dest + 1, c);
		} else {
			avr_ldi(rat_scratch_reg(rat), 0, c);
			avr_push(rat_scratch_reg(rat), c);
		}
	}

	avr_push(reg_dest, c);
}
