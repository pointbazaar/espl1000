#include <stdio.h>

#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "rat/rat.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_store_const_addr(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const int reg_src = rat_get_register(rat, tac_arg1(tac));

	const uint32_t addr = tac_const_value(tac);

	char* c = "TAC_STORE_CONST_ADDR";

	sts(addr, reg_src, c);

	if (rat_is_wide(rat, tac_arg1(tac))) {
		sts(addr + 1, reg_src + 1, c);
	}
}
