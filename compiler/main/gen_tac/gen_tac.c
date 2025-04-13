#include "gen_tac.h"

#include "cli/flags/flags.h"
#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "util/ctx.h"
#include "tac/tacbuffer.h"

bool tac_method(struct TACBuffer* buffer, struct Method* m, struct Ctx* ctx) {

	const uint32_t index = sst_index_of(ctx_tables(ctx)->sst, m->decl->name);

	//first the label of the function
	tacbuffer_append(buffer, makeTACLabelFunction(index));

	//setup the stack frame
	if (flags_x86(ctx_flags(ctx))) {
		tacbuffer_append(buffer, makeTACSetupStackframe(lvst_stack_frame_size_x86(ctx_tables(ctx)->lvst)));
	} else {
		tacbuffer_append(buffer, makeTACSetupStackframe(lvst_stack_frame_size_avr(ctx_tables(ctx)->lvst)));
	}

	return tac_stmtblock(buffer, m->block, ctx);
}

bool tac_stmtblock(struct TACBuffer* buffer, struct StmtBlock* block, struct Ctx* ctx) {
	for (size_t k = 0; k < block->count; k++) {
		if (!tac_stmt(buffer, block->stmts[k], ctx)) {
			return false;
		}
	}

	return true;
}
