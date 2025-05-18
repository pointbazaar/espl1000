#include <stdio.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_goto(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_IF_GOTO";

	const int reg = rat_get_register(rat, tac_arg1(tac));
	char str[32];
	sprintf(str, "L%d", tac_label_index(tac));

	char tmp_label[32];
	sprintf(tmp_label, "L%d", make_label());

	avr_tst(reg, c); //test if r%d is zero

	avr_breq(tmp_label, c);
	avr_jmp(str);
	avr_label(tmp_label);

	// brne(str, c);
	// we cannot use brne since it can only branch +/- 64 words
	// from the instruction pointer

	// since a bool is only ever 1 byte
	// there is no case for wide argument
}
