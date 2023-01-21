#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_cmp_goto(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){

	char* c = "TAC_IF_CMP_GOTO";

	const int reg1 = rat_get_register(rat, tac->dest);
	const int reg2 = rat_get_register(rat, tac->arg1);

	bool wide = rat_is_wide(rat, tac->dest);

	cp(reg1, reg2, c);

	if(wide)
	  cp(reg1+1, reg2+1, c);

	char str[32];
	sprintf(str, "L%d", tac->label_index);

	switch(tac->op){
		case TAC_OP_CMP_EQ: breq(str, c); break;
		case TAC_OP_CMP_NEQ: brne(str, c); break;
		case TAC_OP_CMP_GE: brge(str, c); break;
		case TAC_OP_CMP_LT: brlt(str, c); break;
		default: break;
	}
}
