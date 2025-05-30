#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

// @returns false on error
static bool tac_expr_part_2_no_constvalue(struct TACBuffer* buffer, struct Expr* expr, uint32_t t1, struct Ctx* ctx);
static enum TAC_OP op_to_tac_op(enum OP op_str, bool* reverse_operands);

bool tac_expr(struct TACBuffer* buffer, struct Expr* expr, struct Ctx* ctx) {

	bool success = tac_unopterm(buffer, expr->term1, ctx);

	if (!success) {
		return false;
	}

	if (expr->term2 != NULL) {

		//we know dest of term1
		const uint32_t t1 = tacbuffer_last_dest(buffer);

		if (!tac_expr_part_2_no_constvalue(buffer, expr, t1, ctx)) {
			return false;
		}
	}

	return true;
}

// of course this only works if there is only one term,
// and even then it depends on what kind of term that is
bool tac_expr_addr(struct TACBuffer* buffer, struct Expr* expr, struct Ctx* ctx) {

	assert(expr->term2 == NULL);

	return tac_unopterm_addr(buffer, expr->term1, ctx);
}

static enum TAC_OP op_to_tac_op(enum OP o, bool* reverse_operands) {
	enum TAC_OP op = op = TAC_OP_NONE;
	*reverse_operands = false;

	switch (o) {
		case OP_PLUS: op = TAC_OP_ADD; break;
		case OP_MINUS: op = TAC_OP_SUB; break;
		case OP_MULTIPLY: op = TAC_OP_MUL; break;
		case OP_DIV: op = TAC_OP_DIV; break;
		case OP_MOD: op = TAC_OP_MOD; break;

		case OP_AND: op = TAC_OP_AND; break;
		case OP_OR: op = TAC_OP_OR; break;
		case OP_XOR: op = TAC_OP_XOR; break;

		case OP_SHIFT_LEFT: op = TAC_OP_SHIFT_LEFT; break;
		case OP_SHIFT_RIGHT: op = TAC_OP_SHIFT_RIGHT; break;

		case OP_LT: op = TAC_OP_CMP_LT; break;
		case OP_LE:
			op = TAC_OP_CMP_GE;
			*reverse_operands = true;
			break;

		case OP_GT:
			op = TAC_OP_CMP_LT;
			*reverse_operands = true;
			break;

		case OP_GE: op = TAC_OP_CMP_GE; break;

		case OP_EQ: op = TAC_OP_CMP_EQ; break;
		case OP_NEQ: op = TAC_OP_CMP_NEQ; break;

		case OP_NONE:
		default:
			fprintf(stderr, "error in op_to_tac_op\n");
			fprintf(stderr, "error, op was none of supported TAC_OP_... values\n");
			fprintf(stderr, "op = %d\n", o);
			return TAC_OP_NONE;
	}

	return op;
}

static bool tac_expr_part_2_no_constvalue(struct TACBuffer* buffer, struct Expr* expr, uint32_t t1, struct Ctx* ctx) {

	if (!tac_unopterm(buffer, expr->term2, ctx)) {
		return false;
	}
	const uint32_t t2 = tacbuffer_last_dest(buffer);

	bool reverse_operands = false;

	enum TAC_OP op = op_to_tac_op(expr->op, &reverse_operands);

	if (op == TAC_OP_NONE) {
		return false;
	}

	struct TAC* t;

	if (reverse_operands)
		t = makeTACBinOp(expr->super.line_num, t2, op, t1);
	else
		t = makeTACBinOp(expr->super.line_num, t1, op, t2);

	tacbuffer_append(buffer, t);

	return true;
}
