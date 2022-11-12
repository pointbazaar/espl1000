#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "util/ctx.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_store(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {
	
	const int reg_dest = rat_get_register(rat, tac->dest);

	const int reg_src = rat_get_register(rat, tac->arg1);
	
	//set X 
	mov(XL, reg_dest);
	ldi(XH, 0, "TAC_STORE");
	
	stX(reg_src, "TAC_STORE");
	//st X, reg_src
	//st X, r%d; writeback locals reg_src
}
