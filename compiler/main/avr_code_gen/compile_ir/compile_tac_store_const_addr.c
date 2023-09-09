#include <stdio.h>

#include "avr_code_gen/compile_ir/compile_tac.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

void compile_tac_store_const_addr(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const int reg_src = rat_get_register(rat, tac->arg1);

	const uint32_t addr = tac->const_value;

	sts(addr, reg_src, "TAC_STORE_CONST_ADDR");
}
