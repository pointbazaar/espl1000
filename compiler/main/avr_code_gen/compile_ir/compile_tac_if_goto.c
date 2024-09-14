#include <stdio.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_goto(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_IF_GOTO";

	const int reg = rat_get_register(rat, tac->arg1);
	char str[32];
	sprintf(str, "L%d", tac->label_index);

	char tmp_label[32];
	sprintf(tmp_label, "L%d", make_label());

	tst(reg, c); //test if r%d is zero

	breq(tmp_label, c);
	jmp(str);
	label(tmp_label);

	// brne(str, c);
	// we cannot use brne since it can only branch +/- 64 words
	// from the instruction pointer

	// since a bool is only ever 1 byte
	// there is no case for wide argument
}
