#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_unary_op(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	int reg_src = rat_get_register(rat, tac_arg1(tac));

	int reg_dest = rat_get_register(rat, tac_dest(tac));

	bool wide = rat_is_wide(rat, tac_arg1(tac)) && rat_is_wide(rat, tac_dest(tac));

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);

	char* c = "TAC_UNARY_OP";

	switch (tac_op(tac)) {

		case TAC_OP_UNARY_NOT:
		case TAC_OP_UNARY_BITWISE_NEG:
			mov(reg_dest, reg_src, c);
			com(reg_dest, c);

			if (wide) {
				mov(reg_dest + 1, reg_src + 1, c);
				com(reg_dest + 1, c);
			}
			break;

		case TAC_OP_UNARY_MINUS:

			if (wide) {
				mov(reg_dest, reg_src, c);
				mov(reg_dest + 1, reg_src + 1, c);

				com(reg_dest, c);
				com(reg_dest + 1, c);

				ldi(RAT_SCRATCH_REG, 0, c);
				inc(reg_dest, c);
				adc(reg_dest + 1, RAT_SCRATCH_REG, c);
				return;
			}

			mov(reg_dest, reg_src, c);
			neg(reg_dest, c);
			break;
		default:
			break;
	}
}
