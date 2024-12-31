#include <stdio.h>

#include "ibuffer/ibuffer_x86.h"
#include "rat/rat_x86.h"
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"
#include "tac/tac.h"
#include "rat/rat.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_setup_stackframe_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx, char* current_function_name) {

	char* c = "TAC_SETUP_STACKFRAME";
	const size_t stack_frame_size_bytes = tac_const_value(tac);

	// load base pointer
	// rbp = rsp
	mov_regs(rat_base_ptr(rat), rat_stack_ptr(rat), c);

	if (stack_frame_size_bytes == 0) {
		return;
	}

	// create stack frame, without overwriting the memory
	mov_const(rat_scratch_reg(rat), stack_frame_size_bytes, c);
	// sub rsp, $stack_frame_size
	sub(rat_stack_ptr(rat), rat_scratch_reg(rat), c);

	//save function arguments
	c = "TAC_SETUP_STACKFRAME (store args)";
	struct SST* sst = ctx_tables(ctx)->sst;
	struct LVST* lvst = ctx_tables(ctx)->lvst;
	const uint32_t arg_count = sst_args_count(sst, current_function_name);

	const size_t local_vars_size = lvst_stack_frame_size_local_vars_x86(lvst);

	mov_const(SD_REG_RAX, local_vars_size, c);
	mov_regs(rat_scratch_reg(rat), rat_base_ptr(rat), c);
	sub(rat_scratch_reg(rat), SD_REG_RAX, c);

	for (size_t i = 0; i < arg_count; i++) {

		enum SD_REGISTER reg = rat_param_reg_x86(i);

		char* arg_name = lvst_arg_at(lvst, 0)->name;

		const uint32_t width = lvst_sizeof_var(lvst, arg_name, true);
		mov_const(SD_REG_RAX, width, c);
		sub(rat_scratch_reg(rat), SD_REG_RAX, c);

		mov_store_width(rat_scratch_reg(rat), reg, width, c);
	}
}
