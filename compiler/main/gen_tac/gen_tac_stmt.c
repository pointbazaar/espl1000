#include "cli/flags/flags.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

bool tac_stmt(struct TACBuffer* buffer, struct Stmt* stmt, struct Ctx* ctx) {

	if (flags_debug(ctx_flags(ctx))) {
		printf("[debug] %s: line %d\n", __func__, stmt->super.line_num);
	}

	if (stmt->is_break) {
		const int32_t label_loop_end = ctx_get_label_loop_end(ctx);
		if (label_loop_end < 0) {
			return false;
		}
		tacbuffer_append(buffer, makeTACGoto(label_loop_end));
		return true;
	}
	if (stmt->is_continue) {
		const int32_t label_loop_start = ctx_get_label_loop_start(ctx);
		if (label_loop_start < 0) {
			return false;
		}
		tacbuffer_append(buffer, makeTACGoto(label_loop_start));
		return true;
	}

	switch (stmt->kind) {
		case 1: return tac_call(buffer, stmt->ptr.m1, ctx); break;
		case 2: return tac_whilestmt(buffer, stmt->ptr.m2, ctx); break;
		case 3: tac_ifstmt(buffer, stmt->ptr.m3, ctx); break;
		case 4: tac_retstmt(buffer, stmt->ptr.m4, ctx); break;
		case 5: tac_assignstmt(buffer, stmt->ptr.m5, ctx); break;
		case 7: return tac_forstmt(buffer, stmt->ptr.m7, ctx); break;
		case 9: tac_massignstmt(buffer, stmt->ptr.m9, ctx); break;

		case 10:
			// local var declaration stmt, needs no code gen
			break;

		default:
			fprintf(stderr, "unhandled case in %s: %d\n", __func__, stmt->kind);
			return false;
	}

	return true;
}
