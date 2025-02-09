#include "cli/flags/flags.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_stmt(struct TACBuffer* buffer, struct Stmt* stmt, struct Ctx* ctx) {

	if (flags_debug(ctx_flags(ctx))) {
		printf("[debug] %s: line %d\n", __func__, stmt->super.line_num);
	}

	if (stmt->is_break) {
		tacbuffer_append(buffer, makeTACGoto(ctx_get_label_loop_end(ctx)));
		return;
	}
	if (stmt->is_continue) {
		tacbuffer_append(buffer, makeTACGoto(ctx_get_label_loop_start(ctx)));
		return;
	}

	switch (stmt->kind) {
		case 1: tac_call(buffer, stmt->ptr.m1, ctx); break;
		case 2: tac_whilestmt(buffer, stmt->ptr.m2, ctx); break;
		case 3: tac_ifstmt(buffer, stmt->ptr.m3, ctx); break;
		case 4: tac_retstmt(buffer, stmt->ptr.m4, ctx); break;
		case 5: tac_assignstmt(buffer, stmt->ptr.m5, ctx); break;
		case 7: tac_forstmt(buffer, stmt->ptr.m7, ctx); break;
		case 9: tac_massignstmt(buffer, stmt->ptr.m9, ctx); break;
	}
}
