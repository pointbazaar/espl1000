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
		//st X, reg_src
		stX(reg_src, c);
	}
}
