#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_cmp_goto(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){
    
    const int reg1 = rat_get_register(rat, tac->dest);
    const int reg2 = rat_get_register(rat, tac->arg1);

	cp(reg1, reg2); 
    
    char str[32];
    sprintf(str, "L%d", tac->label_index);
    
    switch(tac->op){
		case TAC_OP_CMP_EQ: breq(str); break;
		case TAC_OP_CMP_NEQ: brne(str); break;
		case TAC_OP_CMP_GE: brge(str); break;
		case TAC_OP_CMP_LT: brlt(str); break;
		default: break;
	}
}
