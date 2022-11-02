#include <stdio.h>

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_setup_sp(struct TAC* tac, FILE* fout){
	
	if(tac->kind != TAC_SETUP_SP){return;}
	
    //initialize Stack Pointer
    fprintf(fout, "ldi r16, high(RAMEND)\n");
    fprintf(fout, "out SPH, r16\n");
    fprintf(fout, "ldi r16, low(RAMEND)\n");
    fprintf(fout, "out SPL, r16\n");

    fprintf(fout, "\n\n");
}
