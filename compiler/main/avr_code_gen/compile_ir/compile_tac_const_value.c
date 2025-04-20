#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_const_value(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	//for TAC_CONST_VALUE there should have been a higher
	//reg allocated for it's destination tmp

	const int reg = rat_get_register(rat, tac_dest(tac));

	//change to unsigned value here, for shifting
	const uint16_t value = tac_const_value(tac);

	char c[60];
	sprintf(c, "TAC_CONST_VALUE %d (0x%x)", value, value);

	ldi(reg, (value & 0xff), c);

	if (rat_is_wide(rat, tac_dest(tac))) {

		ldi((reg + 1), ((value >> 8) & 0xff), c);
	}
}
