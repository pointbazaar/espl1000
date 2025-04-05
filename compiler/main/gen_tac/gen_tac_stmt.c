#include "cli/flags/flags.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

bool tac_stmt(struct TACBuffer* buffer, struct Stmt* stmt, struct Ctx* ctx) {

	if (flags_debug(ctx_flags(ctx))) {
		printf("[debug] %s: line %d\n", __func__, stmt->super.line_num);
	}

	if (stmt->kind == STMT_KIND_BREAK) {
		const int32_t label_loop_end = ctx_get_label_loop_end(ctx);
		if (label_loop_end < 0) {
			return false;
		}
		tacbuffer_append(buffer, makeTACGoto(label_loop_end));
		return true;
	}
	if (stmt->kind == STMT_KIND_CONTINUE) {
		const int32_t label_loop_start = ctx_get_label_loop_start(ctx);
		if (label_loop_start < 0) {
			return false;
		}
		tacbuffer_append(buffer, makeTACGoto(label_loop_start));
		return true;
	}

	switch (stmt->kind) {
		case STMT_KIND_CALL: return tac_call(buffer, stmt->ptr.call, ctx); break;
		case STMT_KIND_WHILE: return tac_whilestmt(buffer, stmt->ptr.while_stmt, ctx); break;
		case STMT_KIND_IF: tac_ifstmt(buffer, stmt->ptr.if_stmt, ctx); break;
		case STMT_KIND_RETURN: tac_retstmt(buffer, stmt->ptr.return_stmt, ctx); break;
		case STMT_KIND_ASSIGN: tac_assignstmt(buffer, stmt->ptr.assign_stmt, ctx); break;
		case STMT_KIND_FOR: return tac_forstmt(buffer, stmt->ptr.for_stmt, ctx); break;

		case STMT_KIND_LOCAL_VAR_DECL:
			// local var declaration stmt, needs no code gen
			break;

		default:
			fprintf(stderr, "unhandled case in %s: %d\n", __func__, stmt->kind);
			return false;
	}

	return true;
}
