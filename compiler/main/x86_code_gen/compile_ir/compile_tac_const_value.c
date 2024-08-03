#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"

#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_const_value_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	//for TAC_CONST_VALUE there should have been a higher
	//reg allocated for it's destination tmp

	const int reg = rat_get_register(rat, tac->dest);

	//change to unsigned value here, for shifting
	const uint64_t value = tac->const_value;

	char c[30];
	sprintf(c, "TAC_CONST_VALUE %lu (0x%lx)", value, value);

	mov_const(reg, (value & 0xffffffff), c);
}
