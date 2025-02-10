#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "cli/flags/flags.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "gen_tac.h"

static void tac_call_case_sst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx);
static void tac_call_case_lvst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx);

static void tac_call_prep_param(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx, int i, const bool push16) {
	struct Expr* expr = call->args[i];

	tac_expr(buffer, expr, ctx);

	struct TAC* t = makeTACParam(tacbuffer_last_dest(buffer), push16, i);

	tacbuffer_append(buffer, t);
}

static void tac_call_prep_params_case_sst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx, struct MethodDecl* decl) {

	const bool x86 = flags_x86(ctx_flags(ctx));
	for (size_t i = 0; i < call->count_args; i++) {
		const uint32_t param_width = lvst_sizeof_type(decl->args[i]->type, x86);
		assert(param_width <= 8);
		const bool push16 = param_width == 2;

		tac_call_prep_param(buffer, call, ctx, i, push16);
	}
}

static void tac_call_prep_params_case_lvst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx, struct LVSTLine* line) {
	const bool x86 = flags_x86(ctx_flags(ctx));
	for (size_t i = 0; i < call->count_args; i++) {
		struct Type* arg_type = line->type->basic_type->subr_type->arg_types[i];
		const uint32_t param_width = lvst_sizeof_type(arg_type, x86);
		assert(param_width <= 8);
		const bool push16 = param_width == 2;

		tac_call_prep_param(buffer, call, ctx, i, push16);
	}
}

void tac_call(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx) {

	char* fname = call->callable->simple_var->name;
	struct SST* sst = ctx_tables(ctx)->sst;
	struct LVST* lvst = ctx_tables(ctx)->lvst;

	if (sst_contains(sst, fname)) {
		tac_call_case_sst(buffer, call, ctx);
		return;
	}
	if (lvst_contains(lvst, fname)) {
		tac_call_case_lvst(buffer, call, ctx);
		return;
	}

	fprintf(stderr, "did not find symbol '%s' in LVST or SST, cannot call.\n", fname);
	fprintf(stderr, "exiting.");
	exit(1);
}

static void tac_call_case_lvst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx) {

	struct LVST* lvst = ctx_tables(ctx)->lvst;
	char* fname = call->callable->simple_var->name;

	struct LVSTLine* line = lvst_get(lvst, fname);
	tac_call_prep_params_case_lvst(buffer, call, ctx, line);

	const bool x86 = flags_x86(ctx_flags(ctx));
	const uint8_t addr_width = (x86) ? 8 : 2;
	const uint8_t local_width = lvst_sizeof_var(lvst, fname, x86);

	tacbuffer_append(buffer, makeTACLoadLocalAddr(make_temp(), lvst_index_of(lvst, fname), addr_width));

	tacbuffer_append(buffer, makeTACLoad(make_temp(), tacbuffer_last_dest(buffer), local_width));

	tacbuffer_append(buffer, makeTACICall(make_temp(), tacbuffer_last_dest(buffer)));
}

static void tac_call_case_sst(struct TACBuffer* buffer, struct Call* call, struct Ctx* ctx) {

	const char* fname = call->callable->simple_var->name;

	struct SST* sst = ctx_tables(ctx)->sst;
	struct SSTLine* line = sst_get(sst, fname);

	struct MethodDecl* decl = line->method->decl;

	tac_call_prep_params_case_sst(buffer, call, ctx, decl);

	if (call->callable->member_access != NULL) {
		printf("member access calls currently unsupported on avr_code_gen\n");
		exit(1);
	}

	if (call->callable->simple_var->count_indices != 0) {
		printf("calls with indices currently unsupported on avr_code_gen\n");
		exit(1);
	}

	uint32_t index = sst_index_of(sst, fname);

	struct TAC* t2 = makeTACCall(make_temp(), index);

	tacbuffer_append(buffer, t2);
}
