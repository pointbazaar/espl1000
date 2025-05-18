#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "cli/flags/flags.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "tables/sst/sst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "gen_tac.h"

static bool tac_call_case_sst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx);
static bool tac_call_case_lvst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx);

// @returns temporary index where the param is stored
// @returns < 0 on error
static int32_t tac_call_prep_param(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx, int i) {
	if (!tac_expr(buffer, call->args[i], ctx)) {
		return -1;
	}

	return tacbuffer_last_dest(buffer);
}

static bool emit_param_transfer(struct TACBuffer* buffer, uint32_t* param_temps, uint8_t* param_widths, bool is_syscall, size_t count, uint32_t line_num) {

	for (size_t i = 0; i < count; i++) {
		const bool push16 = param_widths[i] == 2;
		struct TAC* t = makeTACParam(line_num, param_temps[i], push16, i, is_syscall);

		if (!t) {
			return false;
		}

		tacbuffer_append(buffer, t);
	}

	return true;
}

// @returns false on error
static bool tac_call_prep_params_case_sst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx, struct MethodDecl* decl) {

	bool status = true;
	struct SST* sst = ctx_tables(ctx)->sst;
	const char* fname = call->callable->simple_var->name;
	struct SSTLine* line = sst_get(sst, fname);

	const bool x86 = flags_x86(ctx_flags(ctx));

	uint32_t* param_temps = calloc(call->count_args, sizeof(uint32_t));

	if (!param_temps) {
		return false;
	}

	uint8_t* param_widths = calloc(call->count_args, sizeof(uint8_t));

	if (!param_widths) {
		free(param_temps);
		return false;
	}

	for (size_t i = 0; i < call->count_args; i++) {
		const uint32_t param_width = lvst_sizeof_type(decl->args[i]->type, x86);
		assert(param_width <= 8);

		const int32_t tmp_param = tac_call_prep_param(buffer, call, ctx, i);
		if (tmp_param < 0) {
			status = false;
			goto exit;
		}

		param_temps[i] = tmp_param;
		param_widths[i] = param_width;
	}

	status = emit_param_transfer(buffer, param_temps, param_widths, line->is_syscall, call->count_args, call->super.line_num);

exit:
	free(param_temps);
	free(param_widths);
	return status;
}

// @returns false on error
static bool tac_call_prep_params_case_lvst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx, struct LVSTLine* line) {

	bool status = true;
	const bool x86 = flags_x86(ctx_flags(ctx));

	struct SubrType* subr_type = line->type->basic_type->subr_type;

	uint32_t* param_temps = calloc(call->count_args, sizeof(uint32_t));

	if (!param_temps) {
		return false;
	}

	uint8_t* param_widths = calloc(call->count_args, sizeof(uint8_t));

	if (!param_widths) {
		free(param_temps);
		return false;
	}

	for (size_t i = 0; i < call->count_args; i++) {
		struct Type* arg_type = subr_type->arg_types[i];
		const uint32_t param_width = lvst_sizeof_type(arg_type, x86);
		assert(param_width <= 8);

		const int32_t tmp_param = tac_call_prep_param(buffer, call, ctx, i);
		if (tmp_param < 0) {
			status = false;
			goto exit;
		}

		param_temps[i] = tmp_param;
		param_widths[i] = param_width;
	}

	// TODO: currently the information of syscall / no syscall
	// is lost when passing it as function pointer.
	status = emit_param_transfer(buffer, param_temps, param_widths, false, call->count_args, call->super.line_num);

exit:
	free(param_temps);
	free(param_widths);
	return status;
}

bool tac_call(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx) {

	const char* fname = call->callable->simple_var->name;
	struct SST* sst = ctx_tables(ctx)->sst;
	struct LVST* lvst = ctx_tables(ctx)->lvst;

	if (sst_contains(sst, fname)) {
		return tac_call_case_sst(buffer, call, ctx);
	}
	if (lvst_contains(lvst, fname)) {
		return tac_call_case_lvst(buffer, call, ctx);
	}

	fprintf(stderr, "did not find symbol '%s' in LVST or SST, cannot call.\n", fname);
	return false;
}

static bool tac_call_case_lvst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx) {

	struct LVST* lvst = ctx_tables(ctx)->lvst;
	const char* fname = call->callable->simple_var->name;

	struct LVSTLine* line = lvst_get(lvst, fname);

	if (!tac_call_prep_params_case_lvst(buffer, call, ctx, line)) {
		return false;
	}

	const bool x86 = flags_x86(ctx_flags(ctx));
	const uint8_t addr_width = (x86) ? 8 : 2;
	const uint8_t local_width = lvst_sizeof_var(lvst, fname, x86);

	tacbuffer_append(buffer, makeTACLoadLocalAddr(call->super.line_num, make_temp(), lvst_index_of(lvst, fname), addr_width));

	tacbuffer_append(buffer, makeTACLoad(call->super.line_num, make_temp(), tacbuffer_last_dest(buffer), local_width));

	tacbuffer_append(buffer, makeTACICall(call->super.line_num, make_temp(), tacbuffer_last_dest(buffer)));

	return true;
}

static bool tac_call_case_sst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx) {

	const char* fname = call->callable->simple_var->name;

	struct SST* sst = ctx_tables(ctx)->sst;
	struct SSTLine* line = sst_get(sst, fname);

	struct MethodDecl* decl = line->method->decl;

	if (!tac_call_prep_params_case_sst(buffer, call, ctx, decl)) {
		return false;
	}

	if (call->callable->member_access != NULL) {
		printf("member access calls currently unsupported on avr_code_gen\n");
		return false;
	}

	if (call->callable->simple_var->count_indices != 0) {
		printf("calls with indices currently unsupported on avr_code_gen\n");
		return false;
	}

	uint32_t index = sst_index_of(sst, fname);

	struct TAC* t2 = makeTACCall(call->super.line_num, make_temp(), index);

	tacbuffer_append(buffer, t2);

	return true;
}
