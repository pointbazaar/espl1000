#include <stdio.h>

#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_setup_stackframe_x86(struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_SETUP_STACKFRAME";
	const int stack_frame_size = tac->const_value;

	//push onto the stack to create the stack frame
	for (int k = 0; k < stack_frame_size; k++) {
		// create frame
		push(0, c);
		//it is irrelevant what we push here
	}

	// load base pointer
	// rbp = rsp
	mov_regs(7, 6, c);
}
