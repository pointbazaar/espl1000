#include <stdlib.h>
#include <stdio.h>

#include "cg_avr_basic_block.h"

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

#include "ibuffer/ibuffer.h"

#include "cli/flags/flags.h"

bool emit_asm_avr_single_tac(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	struct ST* st = ctx_tables(ctx);

	if (flags_debug(ctx_flags(ctx))) {
		printf("emit_asm_avr_single_tac %s\n", tac_tostring(tac, st->sst, st->lvst));
	}

	switch (tac_kind(tac)) {

		case TAC_LABEL_INDEXED:
		case TAC_LABEL_FUNCTION: compile_tac_label(tac, ibu, ctx); break;
		case TAC_GOTO: compile_tac_goto(tac, ibu); break;
		case TAC_NOP: compile_tac_nop(ibu); break;
		case TAC_BINARY_OP: return compile_tac_binary_op(rat, tac, ibu); break;
		case TAC_UNARY_OP: compile_tac_unary_op(rat, tac, ibu); break;

		case TAC_IF_GOTO: compile_tac_if_goto(rat, tac, ibu); break;
		case TAC_IF_CMP_GOTO: compile_tac_if_cmp_goto(rat, tac, ibu); break;

		case TAC_COPY: compile_tac_copy(rat, tac, ibu); break;
		case TAC_CONST_VALUE: compile_tac_const_value(rat, tac, ibu); break;
		case TAC_CALL: compile_tac_call(rat, tac, ibu, ctx); break;
		case TAC_PARAM: compile_tac_param(rat, tac, ibu); break;
		case TAC_RETURN: compile_tac_return(rat, tac, ctx, ibu); break;

		case TAC_SETUP_STACKFRAME: compile_tac_setup_stackframe(tac, ibu); break;
		case TAC_SETUP_SP: compile_tac_setup_sp(rat, ibu); break;

		case TAC_LOAD_LOCAL_ADDR: compile_tac_load_local_addr(rat, tac, ctx, ibu); break;
		case TAC_STORE_LOCAL: compile_tac_store_local(rat, tac, ctx, ibu); break;

		case TAC_LOAD: compile_tac_load(rat, tac, ibu); break;
		case TAC_STORE: compile_tac_store(rat, tac, ibu); break;

		default:
			fprintf(stderr, "%s: unhandled case %d\n", __func__, tac_kind(tac));
			return false;
	}
	return true;
}
