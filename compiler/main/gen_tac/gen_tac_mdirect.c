
#include <stdio.h>
#include <stdlib.h>
#include "gen_tac.h"
#include "tac/tacbuffer.h"

static void case_const_addr(struct TACBuffer* buffer, struct Expr* expr);
static void case_variable_addr(struct TACBuffer* buffer, struct Expr* expr, struct Ctx* ctx);

void tac_mdirect(struct TACBuffer* buffer, struct MDirect* m, struct Ctx* ctx) {

	struct Expr* expr = m->expr;

	if (expr->term1->term->kind == 12) {
		case_const_addr(buffer, expr);
	} else {
		case_variable_addr(buffer, expr, ctx);
	}
}

static void case_const_addr(struct TACBuffer* buffer, struct Expr* expr) {

	const uint32_t addr = expr->term1->term->ptr.m12->ptr.m2_int_const;

	//TODO: apply the correct width
	const uint8_t width = 2;

	tacbuffer_append(buffer, makeTACConst(make_temp(), addr));
	tacbuffer_append(buffer, makeTACLoad(make_temp(), tacbuffer_last_dest(buffer), width));
}

static void case_variable_addr(struct TACBuffer* buffer, struct Expr* expr, struct Ctx* ctx) {

	//t1 = [expr]

	tac_expr(buffer, expr, ctx);

	// TODO: for x86, this needs to be 8 bytes, not 2
	struct TAC* t = makeTACLoad(make_temp(), tacbuffer_last_dest(buffer), 2);

	tacbuffer_append(buffer, t);
}
