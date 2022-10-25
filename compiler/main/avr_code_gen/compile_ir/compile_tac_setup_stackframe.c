#include <stdio.h>

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_setup_stackframe(struct TAC* tac, FILE* fout){
	
	const int stack_frame_size = tac->const_value;
	
    //push onto the stack to create the stack frame
    for(int k=0; k < stack_frame_size; k++){
        fprintf(fout, "push r0  ;create frame\n"); 
        //it is irrelevant what we push here
    }
    fprintf(fout, "\n");
	
	//now load X as our base pointer for the stack frame
    fprintf(fout, "in r28, SPL  ;Y is base ptr\n");
    fprintf(fout, "in r29, SPH  ;Y is base ptr\n\n");
}
