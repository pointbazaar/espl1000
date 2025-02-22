#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

// @returns false on error
static bool case_default(struct TACBuffer* buffer, struct WhileStmt* w, struct Ctx* ctx);

// @returns false on error
static bool case_loop(struct TACBuffer* buffer, struct WhileStmt* w, struct Ctx* ctx);

bool tac_whilestmt(struct TACBuffer* buffer, struct WhileStmt* w, struct Ctx* ctx) {

	struct Expr* e = w->condition;

	if (e->term2 == NULL) {
		struct UnOpTerm* u = e->term1;
		if (u->op == OP_NONE) {
			struct Term* t = u->term;
			if (t->kind == 12) {
				//const value, "while my_const { ... }"
				//and the const ist known at compile time so we can
				//just decide if we emit infinite loop or skip this block

				struct ConstValue* c = t->ptr.m12;
				bool inf = false;

				switch (c->kind) {
					case 1: inf = c->ptr.m1_bool_const; break;
					case 2: inf = c->ptr.m2_int_const; break;
					case 3: inf = c->ptr.m3_char_const; break;
					case 5: inf = c->ptr.m5_hex_const; break;
					case 6: inf = c->ptr.m5_hex_const; break;
				}

				if (inf) {
					return case_loop(buffer, w, ctx);
				}

				//otherwise, we don't need to emit it at all,
				//since it's never going to be executed

			} else {
				return case_default(buffer, w, ctx);
			}
		}
	} else {
		//both terms present

		return case_default(buffer, w, ctx);
	}

	return true;
}

static bool case_loop(struct TACBuffer* buffer, struct WhileStmt* w, struct Ctx* ctx) {

	//for a constant true condition (still able to 'break' out though)

	//L0:
	// block
	//goto L0
	//Lend:

	uint32_t l0 = make_label();
	uint32_t lend = make_label();

	ctx_enter_loop(ctx, l0, lend);

	tacbuffer_append(buffer, makeTACLabel(l0));

	tac_stmtblock(buffer, w->block, ctx);

	tacbuffer_append(buffer, makeTACGoto(l0));

	tacbuffer_append(buffer, makeTACLabel(lend));

	return ctx_exit_loop(ctx);
}

static bool case_default(struct TACBuffer* buffer, struct WhileStmt* w, struct Ctx* ctx) {
	//L0:
	//t1 = expr
	//if-goto t1 L1
	//goto Lend:
	//L1:
	// block
	// goto L0
	//Lend:

	uint32_t l0 = make_label();
	uint32_t l1 = make_label();
	uint32_t lend = make_label();

	ctx_enter_loop(ctx, l0, lend);

	tacbuffer_append(buffer, makeTACLabel(l0));

	tac_expr(buffer, w->condition, ctx);

	struct TAC* t = makeTACIfGoto(tacbuffer_last_dest(buffer), l1);
	tacbuffer_append(buffer, t);

	tacbuffer_append(buffer, makeTACGoto(lend));

	tacbuffer_append(buffer, makeTACLabel(l1));

	tac_stmtblock(buffer, w->block, ctx);

	tacbuffer_append(buffer, makeTACGoto(l0));

	tacbuffer_append(buffer, makeTACLabel(lend));

	return ctx_exit_loop(ctx);
}
