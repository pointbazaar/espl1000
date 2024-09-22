#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_param(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_PARAM";

	const int reg_dest = rat_get_register(rat, tac->dest);

	assert(tac->const_value % 8 == 0);
	assert(tac->const_value <= 64);

	if (tac->const_value == 16) {
		// this means it's a 16 bit parameter
		// so we must push twice

		//TODO: ensure that the temporary has the correct width in RAT
		//assert(rat_is_wide(rat, tac->dest));

		if (rat_is_wide(rat, tac->dest)) {
			push(reg_dest + 1, c);
		} else {
			ldi(rat_scratch_reg(rat), 0, c);
			push(rat_scratch_reg(rat), c);
		}
	}

	push(reg_dest, c);
}
