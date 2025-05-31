#include <stdio.h>
#include <stdlib.h>

#include "cg_x86_basic_block.h"

#include "rat/rat.h"
#include "tables/symtable/symtable.h"
#include "tables/sst/sst.h"
#include "tables/lvst/lvst.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

#include "ibuffer/ibuffer_x86.h"

#include "cli/flags/flags.h"

bool emit_asm_x86_single_tac(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu, char* current_function_name) {

	struct ST* st = ctx_tables(ctx);

	if (flags_debug(ctx_flags(ctx))) {
		printf("emit_asm_x86_single_tac %s\n", tac_tostring(tac, st->sst, st->lvst));
	}

	ibu_set_line_num(ibu, tac_line_num(tac));

	switch (tac_kind(tac)) {

		case TAC_LABEL_INDEXED:
		case TAC_LABEL_FUNCTION: return compile_tac_label_x86(tac, ibu, ctx); break;
		case TAC_GOTO: compile_tac_goto_x86(tac, ibu); break;
		case TAC_NOP: compile_tac_nop_x86(ibu); break;
		case TAC_BINARY_OP: compile_tac_binary_op_x86(rat, tac, ibu); break;
		case TAC_UNARY_OP: compile_tac_unary_op_x86(rat, tac, ibu); break;

		case TAC_IF_GOTO: compile_tac_if_goto_x86(rat, tac, ibu); break;
		case TAC_IF_CMP_GOTO: compile_tac_if_cmp_goto_x86(rat, tac, ibu); break;

		case TAC_COPY: compile_tac_copy_x86(rat, tac, ibu); break;
		case TAC_CONST_VALUE: compile_tac_const_value_x86(rat, tac, ibu); break;
		case TAC_CONST_DATA: compile_tac_const_data_x86(rat, tac, ctx, ibu); break;
		case TAC_CALL: compile_tac_call_x86(rat, tac, ibu, ctx, current_function_name); break;
		case TAC_ICALL: compile_tac_icall_x86(rat, tac, ibu, ctx, current_function_name); break;
		case TAC_PARAM: compile_tac_param_x86(rat, tac, ibu); break;
		case TAC_RETURN: compile_tac_return_x86(rat, tac, ctx, ibu); break;

		case TAC_SETUP_STACKFRAME: compile_tac_setup_stackframe_x86(rat, tac, ibu, ctx, current_function_name); break;

		case TAC_LOAD_LOCAL_ADDR: compile_tac_load_local_addr_x86(rat, tac, ctx, ibu); break;
		case TAC_LOAD_FUNCTION_PTR: compile_tac_load_function_ptr_x86(rat, tac, ctx, ibu); break;
		case TAC_STORE_LOCAL: compile_tac_store_local_x86(rat, tac, ctx, ibu); break;

		case TAC_LOAD: compile_tac_load_x86(rat, tac, ibu); break;
		case TAC_STORE: compile_tac_store_x86(rat, tac, ibu); break;

		default:
			fprintf(stderr, "tac->kind == 0x%x (%d) not handled in %s\n", tac_kind(tac), tac_kind(tac), __FUNCTION__);
			return false;
	}

	return true;
}
