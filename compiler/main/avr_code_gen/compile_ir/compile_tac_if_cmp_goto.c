#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_cmp_goto(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_IF_CMP_GOTO";

	const int reg1 = rat_get_register(rat, tac_dest(tac));
	const int reg2 = rat_get_register(rat, tac_arg1(tac));

	bool wide = rat_is_wide(rat, tac_dest(tac));

	avr_cp(reg1, reg2, c);

	if (wide)
		avr_cp(reg1 + 1, reg2 + 1, c);

	char str[32];
	sprintf(str, "L%d", tac_label_index(tac));

	char tmp_label[32];
	sprintf(tmp_label, "L%d", make_label());

	// we invert the condition here so that we use 'jmp' for the jump to our target label.
	// We cannot use 'brne', 'brlt', 'breq', ... as they can only jump +/- 64 words
	// and we do not know how far away our target label is.

	switch (tac_op(tac)) {
		case TAC_OP_CMP_EQ: avr_brne(tmp_label, c); break;
		case TAC_OP_CMP_NEQ: avr_breq(tmp_label, c); break;
		case TAC_OP_CMP_GE: avr_brlt(tmp_label, c); break;
		case TAC_OP_CMP_LT: avr_brge(tmp_label, c); break;
		default: break;
	}

	avr_jmp(str);
	avr_label(tmp_label);
}
