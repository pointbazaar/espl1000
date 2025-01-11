#include <stdio.h>

#include "ibuffer/ibuffer_x86.h"
#include "rat/rat_x86.h"
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"
#include "tac/tac.h"
#include "rat/rat.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

static void callee_save_registers(struct RAT* rat, struct IBuffer* ibu) {

	char* c = "TAC_SETUP_STACKFRAME (callee saved regs)";
	for (enum SD_REGISTER reg = SD_REG_RBX; reg < SD_REG_END_X86; reg++) {
		if (rat_callee_must_save(rat, reg)) {
			push(reg, c);
		}
	}
}

static void save_function_args_into_stack_frame(struct RAT* rat, struct IBuffer* ibu, struct LVST* lvst, struct SST* sst, char* current_function_name) {

	//save function arguments
	char c[100];
	sprintf(c, "TAC_SETUP_STACKFRAME (store args)");

	const uint32_t arg_count = sst_args_count(sst, current_function_name);

	const size_t local_vars_size = lvst_stack_frame_size_local_vars_x86(lvst);

	mov_const(SD_REG_RAX, local_vars_size, c);
	mov_regs(rat_scratch_reg(rat), rat_base_ptr(rat), c);

	if (local_vars_size) {
		sub(rat_scratch_reg(rat), SD_REG_RAX, c);
	}

	for (size_t i = 0; i < arg_count; i++) {

		sprintf(c, "TAC_SETUP_STACKFRAME (store arg %ld)", i);

		enum SD_REGISTER reg = rat_param_reg_x86(i);

		char* arg_name = lvst_arg_at(lvst, i)->name;

		const uint32_t width = lvst_sizeof_var(lvst, arg_name, true);
		mov_const(SD_REG_RAX, width, c);
		sub(rat_scratch_reg(rat), SD_REG_RAX, c);

		mov_store_width(rat_scratch_reg(rat), reg, width, c);
	}
}

void compile_tac_setup_stackframe_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx, char* current_function_name) {

	char* c = "TAC_SETUP_STACKFRAME";

	callee_save_registers(rat, ibu);

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

	struct SST* sst = ctx_tables(ctx)->sst;
	struct LVST* lvst = ctx_tables(ctx)->lvst;

	save_function_args_into_stack_frame(rat, ibu, lvst, sst, current_function_name);
}
