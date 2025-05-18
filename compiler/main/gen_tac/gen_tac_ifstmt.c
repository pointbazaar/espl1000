#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

static bool tac_ifstmt_1_block(struct TACBuffer* buffer, struct IfStmt* s, struct Ctx* ctx);
static bool tac_ifstmt_2_block(struct TACBuffer* buffer, struct IfStmt* s, struct Ctx* ctx);

bool tac_ifstmt(struct TACBuffer* buffer, struct IfStmt* s, struct Ctx* ctx) {

	if (s->else_block == NULL)
		return tac_ifstmt_1_block(buffer, s, ctx);
	else
		return tac_ifstmt_2_block(buffer, s, ctx);
}

static bool tac_ifstmt_1_block(struct TACBuffer* buffer, struct IfStmt* s, struct Ctx* ctx) {

	//t1 = expr
	//if-goto t1 ltrue
	//goto lend
	// ltrue:
	//  if block
	//lend:

	uint32_t lend = make_label();
	uint32_t ltrue = make_label();

	if (!tac_expr(buffer, s->condition, ctx)) {
		return false;
	}

	struct TAC* t_if_goto_end = makeTACIfGoto(s->super.line_num, tacbuffer_last_dest(buffer), ltrue);
	tacbuffer_append(buffer, t_if_goto_end);

	tacbuffer_append(buffer, makeTACGoto(s->super.line_num, lend));

	tacbuffer_append(buffer, makeTACLabel(s->super.line_num, ltrue));

	if (!tac_stmtblock(buffer, s->block, ctx)) {
		return false;
	}

	tacbuffer_append(buffer, makeTACGoto(s->super.line_num, lend));

	tacbuffer_append(buffer, makeTACLabel(s->super.line_num, lend));

	return true;
}

static bool tac_ifstmt_2_block(struct TACBuffer* buffer, struct IfStmt* s, struct Ctx* ctx) {
	//t1 = expr
	//if-goto t1 L1
	//goto L2:
	//L1:
	// if block
	// goto end
	//L2:
	// else block
	//end:

	uint32_t l1 = make_label();
	uint32_t l2 = make_label();
	uint32_t lend = make_label();

	if (!tac_expr(buffer, s->condition, ctx)) {
		return false;
	}

	struct TAC* t = makeTACIfGoto(s->super.line_num, tacbuffer_last_dest(buffer), l1);
	tacbuffer_append(buffer, t);

	tacbuffer_append(buffer, makeTACGoto(s->super.line_num, l2));

	tacbuffer_append(buffer, makeTACLabel(s->super.line_num, l1));

	if (!tac_stmtblock(buffer, s->block, ctx)) {
		return false;
	}

	tacbuffer_append(buffer, makeTACGoto(s->super.line_num, lend));

	tacbuffer_append(buffer, makeTACLabel(s->super.line_num, l2));

	if (!tac_stmtblock(buffer, s->else_block, ctx)) {
		return false;
	}

	tacbuffer_append(buffer, makeTACGoto(s->super.line_num, lend));

	tacbuffer_append(buffer, makeTACLabel(s->super.line_num, lend));

	return true;
}
