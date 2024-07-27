#include <stdio.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_goto_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_IF_GOTO";

	const int reg = rat_get_register(rat, tac->arg1);
	char str[32];
	sprintf(str, "L%d", tac->label_index);

	//test if r%d is zero
	const int rscratch = rat_scratch_reg(rat);

	mov_const(rscratch, 0, c);
	cmp(rscratch, reg, c);
	jne(str, c);

	// since a bool is only ever 1 byte
	// there is no case for wide argument
}
