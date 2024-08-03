#include <stdio.h>

#include "cg_x86_basic_block.h"

#include "rat/rat.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

#include "ibuffer/ibuffer_x86.h"

#include "../../cli/flags/flags.h"

void emit_asm_x86_single_tac(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	if (flags_debug(ctx_flags(ctx))) {
		printf("emit_asm_x86_single_tac %s\n", tac_tostring(tac, ctx));
	}

	switch (tac->kind) {

		case TAC_LABEL_INDEXED:
		case TAC_LABEL_FUNCTION: compile_tac_label_x86(tac, ibu, ctx); break;
		case TAC_GOTO: compile_tac_goto_x86(tac, ibu); break;
		case TAC_NOP: compile_tac_nop_x86(ibu); break;
		case TAC_BINARY_OP: compile_tac_binary_op_x86(rat, tac, ibu); break;
		case TAC_UNARY_OP: compile_tac_unary_op_x86(rat, tac, ibu); break;

		case TAC_IF_GOTO: compile_tac_if_goto_x86(rat, tac, ibu); break;
		case TAC_IF_CMP_GOTO: compile_tac_if_cmp_goto_x86(rat, tac, ibu); break;

		case TAC_COPY: compile_tac_copy_x86(rat, tac, ibu); break;
		case TAC_CONST_VALUE: compile_tac_const_value_x86(rat, tac, ibu); break;
		case TAC_CALL: compile_tac_call_x86(rat, tac, ibu, ctx); break;
		case TAC_PARAM: compile_tac_param_x86(rat, tac, ibu); break;
		case TAC_RETURN: compile_tac_return_x86(rat, tac, ctx, ibu); break;
		case TAC_BINARY_OP_IMMEDIATE: compile_tac_binary_op_immediate_x86(rat, tac, ibu); break;
		case TAC_LOAD_CONST_ADDR: compile_tac_load_const_addr_x86(rat, tac, ibu); break;
		case TAC_STORE_CONST_ADDR: compile_tac_store_const_addr_x86(rat, tac, ibu); break;

		case TAC_SETUP_STACKFRAME: compile_tac_setup_stackframe_x86(tac, ibu); break;

		case TAC_LOAD_LOCAL: compile_tac_load_local_x86(rat, tac, ctx, ibu); break;
		case TAC_LOAD_LOCAL_ADDR: compile_tac_load_local_addr_x86(rat, tac, ctx, ibu); break;
		case TAC_STORE_LOCAL: compile_tac_store_local_x86(rat, tac, ctx, ibu); break;

		case TAC_LOAD: compile_tac_load_x86(rat, tac, ibu); break;
		case TAC_STORE: compile_tac_store_x86(rat, tac, ibu); break;

		default:
			printf("tac->kind == 0x%x not handled in %s\n", tac->kind, __FUNCTION__);
			break;
	}
}
