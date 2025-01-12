#include <stdio.h>
#include <stdlib.h>

#include "cli/flags/flags.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

#include "typeinference/typeinfer.h"

static void case_variable_addr(struct TACBuffer* buffer, struct MAssignStmt* m, struct Ctx* ctx, const uint8_t width) {

	//emit code to calculate the expression
	tac_expr(buffer, m->expr, ctx);
	uint32_t texpr = tacbuffer_last_dest(buffer);

	//emit code to calculate the address
	struct Expr* lhs = m->lhs->expr;
	tac_expr(buffer, lhs, ctx);
	uint32_t taddr = tacbuffer_last_dest(buffer);

	tacbuffer_append(buffer, makeTACStore(taddr, texpr, width));
}

void tac_massignstmt(struct TACBuffer* buffer, struct MAssignStmt* m, struct Ctx* ctx) {

	struct Expr* lhs = m->lhs->expr;

	struct Type* expr_type = infer_type_expr(ctx_tables(ctx), m->expr);
	const bool x86 = flags_x86(ctx_flags(ctx));
	const uint8_t width = lvst_sizeof_type(expr_type, x86);

	case_variable_addr(buffer, m, ctx, width);
}
