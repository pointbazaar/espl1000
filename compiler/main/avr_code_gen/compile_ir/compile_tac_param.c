#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_param(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){

	const char* c = "TAC_PARAM";

	const int reg_dest = rat_get_register(rat, tac->dest);

	push(reg_dest, c);

	if(rat_is_wide(rat, tac->dest))
		push(reg_dest+1, c);
}
