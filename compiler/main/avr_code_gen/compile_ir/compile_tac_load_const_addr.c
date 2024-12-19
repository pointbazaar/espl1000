#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "rat/rat.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_load_const_addr(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const int reg_dest = rat_get_register(rat, tac->dest);

	const bool is_wide = rat_is_wide(rat, tac->dest);

	const uint32_t addr = tac->const_value;

	lds(reg_dest, addr, "TAC_LOAD_CONST_ADDR");

	if (is_wide) {
		lds(reg_dest + 1, addr + 1, "TAC_LOAD_CONST_ADDR");
	}
}
