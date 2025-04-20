#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "cli/flags/flags.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "typechecker/util/tc_utils.h"
#include "typeinference/typeinfer.h"

#include "gen_tac.h"

static bool case_default(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx);
static bool case_indices(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx, const uint8_t width);
static bool case_member(struct TACBuffer* buf, struct AssignStmt* a, struct Ctx* ctx, const uint8_t width);
static bool case_variable(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx);

bool tac_assignstmt(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx) {

	if (flags_debug(ctx_flags(ctx))) {
		printf("[debug] %s\n", __func__);
	}

	assert(a->expr);
	if (!tac_expr(buffer, a->expr, ctx)) {
		return false;
	}
	const uint32_t texpr = tacbuffer_last_dest(buffer);

	struct Type* expr_type = infer_type_expr(ctx_tables(ctx), a->expr);
	assert(expr_type != NULL);
	const bool x86 = flags_x86(ctx_flags(ctx));
	const uint8_t width = lvst_sizeof_type(expr_type, x86);

	if (a->lvalue->var) {
		return case_variable(buffer, a, ctx);
	}
	if (a->lvalue->deref) {

		if (!tac_term(buffer, a->lvalue->deref->term, ctx)) {
			return false;
		}
		const uint32_t taddr = tacbuffer_last_dest(buffer);

		tacbuffer_append(buffer, makeTACStore(taddr, texpr, width));
		return true;
	}

	assert(false);

	return true;
}

static bool case_variable(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx) {

	struct Type* member_type = infer_type_variable(ctx_tables(ctx), a->lvalue->var);
	assert(member_type != NULL);

	const bool x86 = flags_x86(ctx_flags(ctx));
	const uint8_t width = lvst_sizeof_type(member_type, x86);

	if (a->lvalue->var->member_access != NULL) {
		return case_member(buffer, a, ctx, width);
	} else if (a->lvalue->var->simple_var->count_indices != 0) {
		return case_indices(buffer, a, ctx, width);
	} else {
		return case_default(buffer, a, ctx);
	}
}

static bool case_default(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx) {

	const uint32_t local_index = lvst_index_of(ctx_tables(ctx)->lvst, a->lvalue->var->simple_var->name);

	struct TAC* t = makeTACStoreLocal(
	    local_index,
	    tacbuffer_last_dest(buffer));

	tacbuffer_append(buffer, t);

	return true;
}

static bool case_indices(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx, const uint8_t width) {

	struct LVST* lvst = ctx_tables(ctx)->lvst;

	assert(a->lvalue->var);

	const uint32_t local_index = lvst_index_of(lvst, a->lvalue->var->simple_var->name);
	const bool x86 = flags_x86(ctx_flags(ctx));
	const uint8_t addr_width = (x86) ? 8 : 2;
	const uint8_t local_width = lvst_sizeof_var(lvst, a->lvalue->var->simple_var->name, x86);

	//texpr = ...
	//toffset = offset due to index 0
	//local_index = ...

	uint32_t texpr = tacbuffer_last_dest(buffer);

	//load t1 = local index
	uint32_t t1 = make_temp();
	tacbuffer_append(buffer, makeTACLoadLocalAddr(make_temp(), local_index, addr_width));
	tacbuffer_append(buffer, makeTACLoad(t1, tacbuffer_last_dest(buffer), local_width));

	for (int i = 0; i < a->lvalue->var->simple_var->count_indices; i++) {
		//calculate offset due to index
		//toffset
		if (!tac_expr(buffer, a->lvalue->var->simple_var->indices[0], ctx)) {
			return false;
		}
		uint32_t toffset = tacbuffer_last_dest(buffer);

		//add offset, t1 += toffset
		tacbuffer_append(buffer, makeTACBinOp(t1, TAC_OP_ADD, toffset));
	}

	//[t1] = texpr
	tacbuffer_append(buffer, makeTACStore(t1, texpr, width));

	return true;
}

static bool case_member(struct TACBuffer* buf, struct AssignStmt* a, struct Ctx* ctx, const uint8_t width) {

	const uint32_t texpr = tacbuffer_last_dest(buf);

	assert(a->lvalue->var);

	//find out the address of the variable
	tac_variable_addr(buf, a->lvalue->var, ctx, NULL);

	uint32_t taddr = tacbuffer_last_dest(buf);

	tacbuffer_append(buf, makeTACStore(taddr, texpr, width));

	return true;
}
