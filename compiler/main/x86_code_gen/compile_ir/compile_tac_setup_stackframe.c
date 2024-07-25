#include <stdio.h>

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_setup_stackframe(struct TAC* tac, struct IBuffer* ibu) {

	const int stack_frame_size = tac->const_value;

	//push onto the stack to create the stack frame
	for (int k = 0; k < stack_frame_size; k++) {
		//push r0  ;create frame
		push(0, "create stackframe");
		//it is irrelevant what we push here
	}

	//load base pointer Y
	in(YL, SPL, "load frame pointer Y");
	in(YH, SPH, "load frame pointer Y");
}
