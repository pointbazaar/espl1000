#include <stdio.h>

#include "ibuffer/ibuffer_x86.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_setup_stackframe_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	char* c = "TAC_SETUP_STACKFRAME";
	const int stack_frame_size = tac->const_value;
	const size_t stack_frame_size_bytes = stack_frame_size * 8;

	// load base pointer
	// rbp = rsp
	mov_regs(rat_base_ptr(rat), rat_stack_ptr(rat), c);

	// create stack frame, without overwriting the memory
	mov_const(rat_scratch_reg(rat), stack_frame_size_bytes, c);
	// sub rsp, $stack_frame_size
	sub(rat_stack_ptr(rat), rat_scratch_reg(rat), c);
}
