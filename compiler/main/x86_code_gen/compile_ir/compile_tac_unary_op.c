#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_unary_op_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	int reg_src = rat_get_register(rat, tac->arg1);

	int reg_dest = rat_get_register(rat, tac->dest);

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);

	char* c = "TAC_UNARY_OP";

	switch (tac->op) {

		case TAC_OP_UNARY_NOT:
		case TAC_OP_UNARY_BITWISE_NEG:
			mov(reg_dest, reg_src, c);
			com(reg_dest, c);

				break;

		case TAC_OP_UNARY_MINUS:

			mov(reg_dest, reg_src, c);
			neg(reg_dest, c);
			break;
		default:
			break;
	}
}
