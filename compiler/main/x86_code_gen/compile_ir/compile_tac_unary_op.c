#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"
#include "ibuffer/ibuffer_x86.h"

void compile_tac_unary_op_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	int reg_src = rat_get_register(rat, tac_arg1(tac));

	int reg_dest = rat_get_register(rat, tac_dest(tac));

	char* c = "TAC_UNARY_OP";

	switch (tac_op(tac)) {

		case TAC_OP_UNARY_NOT:
			cmp_const(reg_src, 0, c);
			sete(reg_dest, c);
			movzx_regs(reg_dest, reg_dest, 1, c);
			break;

		case TAC_OP_UNARY_BITWISE_NEG:
			mov_regs(reg_dest, reg_src, c);
			not(reg_dest, c);
			break;

		case TAC_OP_UNARY_MINUS:

			mov_regs(reg_dest, reg_src, c);
			neg(reg_dest, c);
			break;
		default:
			break;
	}
}
