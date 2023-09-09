#include <stdio.h>

#include "avr_code_gen/compile_ir/compile_tac.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

void compile_tac_load_const_addr(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const int reg_dest = rat_get_register(rat, tac->dest);

	const uint32_t addr = tac->const_value;

	lds(reg_dest, addr, "TAC_LOAD_CONST_ADDR");
}
