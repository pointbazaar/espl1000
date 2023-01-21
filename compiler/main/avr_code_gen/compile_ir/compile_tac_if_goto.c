#include <stdio.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_goto(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){

	char* c = "TAC_IF_GOTO";

	const int reg = rat_get_register(rat, tac->arg1);
	char str[32];
	sprintf(str, "L%d", tac->label_index);

	const bool wide = rat_is_wide(rat, tac->arg1);

	tst(reg, c); //test if r%d is zero
	brne(str, c);

	if(wide){
		tst(reg+1, c);
		brne(str, c);
	}
}
