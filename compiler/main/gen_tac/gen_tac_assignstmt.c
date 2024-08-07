#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "gen_tac.h"

static void case_default(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx);
static void case_indices(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx);
static void case_member(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx);

void tac_assignstmt(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx) {

	tac_expr(buffer, a->expr, ctx);

	if (a->var->member_access != NULL) {
		case_member(buffer, a, ctx);
	} else if (a->var->simple_var->count_indices != 0) {
		case_indices(buffer, a, ctx);
	} else {
		case_default(buffer, a, ctx);
	}
}

static void case_default(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx) {

	const uint32_t local_index = lvst_index_of(ctx_tables(ctx)->lvst, a->var->simple_var->name);

	struct TAC* t = makeTACStoreLocal(
	    local_index,
	    tacbuffer_last_dest(buffer));

	tacbuffer_append(buffer, t);
}

static void case_indices(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx) {

	const uint32_t local_index = lvst_index_of(ctx_tables(ctx)->lvst, a->var->simple_var->name);

	//texpr = ...
	//toffset = offset due to index 0
	//local_index = ...

	uint32_t texpr = tacbuffer_last_dest(buffer);

	//load t1 = local index
	uint32_t t1 = make_temp();
	tacbuffer_append(buffer, makeTACLoadLocal(t1, local_index));

	for (int i = 0; i < a->var->simple_var->count_indices; i++) {
		//calculate offset due to index
		//toffset
		tac_expr(buffer, a->var->simple_var->indices[0], ctx);
		uint32_t toffset = tacbuffer_last_dest(buffer);

		//add offset, t1 += toffset
		tacbuffer_append(buffer, makeTACBinOp(t1, TAC_OP_ADD, toffset));
	}

	//[t1] = texpr
	tacbuffer_append(buffer, makeTACStore(t1, texpr));
}

static void case_member(struct TACBuffer* buffer, struct AssignStmt* a, struct Ctx* ctx) {

	uint32_t texpr = tacbuffer_last_dest(buffer);

	//find out the address of the variable
	tac_variable_addr(buffer, a->var, ctx);

	uint32_t taddr = tacbuffer_last_dest(buffer);

	tacbuffer_append(buffer, makeTACStore(taddr, texpr));
}
