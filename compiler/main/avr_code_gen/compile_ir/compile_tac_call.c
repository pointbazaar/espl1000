#include <stdio.h>

#include "tac/tac.h"

void compile_tac_call(struct RAT* rat, struct TAC* tac, FILE* fout){
	
	//check if the temp that the returned value has been
	//assigned to has a register
	if(!rat_has_register(rat, tac->dest)){
        int index = rat_get_free_register(rat, false);
        rat_occupy_register(rat, index, tac->dest);
    }

    int reg_dest = rat_get_register(rat, tac->dest);
	
    fprintf(fout, "call %s\n", tac->str);
    
    fprintf(fout, "mov r%d, r0", reg_dest);
}
