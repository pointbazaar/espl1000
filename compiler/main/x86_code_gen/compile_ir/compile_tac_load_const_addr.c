#include <stdio.h>

#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "rat/rat.h"

#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_const_addr_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_LOAD_CONST_ADDR";

	const int reg_dest = rat_get_register(rat, tac_dest(tac));

	const uint64_t addr = tac_const_value(tac);

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);

	mov_const(RAT_SCRATCH_REG, addr, c);
	mov_load(reg_dest, RAT_SCRATCH_REG, c);
}
