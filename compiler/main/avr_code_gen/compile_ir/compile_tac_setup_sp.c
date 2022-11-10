#include <stdio.h>

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_setup_sp(struct TAC* tac, FILE* fout){
	
	if(tac->kind != TAC_SETUP_SP){return;}
	
    //initialize Stack Pointer
    fprintf(fout, "ldi r%d, high(RAMEND)\n", RAT_SCRATCH_REG);
    fprintf(fout, "out SPH, r%d\n", RAT_SCRATCH_REG);
    fprintf(fout, "ldi r%d, low(RAMEND)\n", RAT_SCRATCH_REG);
    fprintf(fout, "out SPL, r%d\n", RAT_SCRATCH_REG);

    fprintf(fout, "\n\n");
}
