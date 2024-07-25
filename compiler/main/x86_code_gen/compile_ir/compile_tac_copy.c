#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_copy(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const int reg_src = rat_get_register(rat, tac->arg1);
	const int reg_dest = rat_get_register(rat, tac->dest);

	mov(reg_dest, reg_src, "TAC_COPY");

	if (rat_is_wide(rat, tac->arg1))
		mov(reg_dest + 1, reg_src + 1, "TAC_COPY");
}
