#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_cmp_goto_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_IF_CMP_GOTO";

	const int reg1 = rat_get_register(rat, tac->dest);
	const int reg2 = rat_get_register(rat, tac->arg1);

	cmp(reg1, reg2, c);

	char str[64];
	sprintf(str, "L%d", tac->label_index);

	switch (tac->op) {
		case TAC_OP_CMP_EQ: je(str, c); break;
		case TAC_OP_CMP_NEQ: jne(str, c); break;
		case TAC_OP_CMP_GE: jge(str, c); break;
		case TAC_OP_CMP_LT: jl(str, c); break;
		default: break;
	}
}
