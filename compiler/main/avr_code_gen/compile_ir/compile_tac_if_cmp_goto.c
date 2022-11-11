#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_if_cmp_goto(struct RAT* rat, struct TAC* tac, FILE* fout){
    
    const int reg1 = rat_get_register(rat, tac->dest);
    const int reg2 = rat_get_register(rat, tac->arg1);

    fprintf(fout, "cp r%d, r%d\n", reg1, reg2); 
    
    char* mnem = "ERR";
    
    switch(tac->op){
		case TAC_OP_CMP_EQ: mnem = "breq"; break;
		case TAC_OP_CMP_NEQ: mnem = "brne"; break;
		case TAC_OP_CMP_GE: mnem = "brge"; break;
		case TAC_OP_CMP_LT: mnem = "brlt"; break;
		default: 
			printf("fatal error in compile_tac_if_cmp_goto.");
			fflush(stdout);
			exit(1);
			break;
	}
	
    fprintf(fout, "%s L%d\n", mnem, tac->label_index);
}
