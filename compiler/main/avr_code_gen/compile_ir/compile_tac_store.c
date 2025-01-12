#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_store(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_STORE";

	const int reg_dest = rat_get_register(rat, tac_dest(tac));

	const int reg_src = rat_get_register(rat, tac_arg1(tac));

	const bool src_wide = rat_is_wide(rat, tac_arg1(tac));
	const bool dest_wide = rat_is_wide(rat, tac_dest(tac));

	const uint8_t store_width = tac_load_store_width(tac);
	assert(store_width <= 2);

	//set X
	mov(XL, reg_dest, c);

	if (dest_wide)
		mov(XH, reg_dest + 1, c);
	else
		ldi(XH, 0, c);

	if (src_wide) {
		//st X+, reg_src
		//st X, reg_src+1
		stXplus(reg_src, c);
		stX(reg_src + 1, c);
	} else {
		if (store_width == 2) {
			// clear the upper byte at the target address
			ldi(rat_scratch_reg(rat), 0, c);
			stXplus(reg_src, c);
			stX(rat_scratch_reg(rat), c);
		} else {
			//st X, reg_src
			stX(reg_src, c);
		}
	}
}
