#include <stdio.h>

#include "tac/tac.h"
#include "rat/rat.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

#include "compile_tac.h"

void compile_tac_setup_sp(struct RAT* rat, struct IBuffer* ibu) {

	//initialize Stack Pointer
	char* comment = "setup SP";

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);

	ldi(RAT_SCRATCH_REG, high(RAMEND), comment);
	out(SPH, RAT_SCRATCH_REG, comment);
	ldi(RAT_SCRATCH_REG, low(RAMEND), comment);
	out(SPL, RAT_SCRATCH_REG, comment);
}
