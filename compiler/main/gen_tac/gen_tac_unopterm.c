#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

bool tac_unopterm(struct TACBuffer* buffer, struct UnOpTerm* u, struct Ctx* ctx) {

	if (u->deref) {
		return tac_deref(buffer, u->deref, ctx);
	}

	if (u->address_of) {
		return tac_address_of(buffer, u->address_of, ctx);
	}

	bool success = tac_term(buffer, u->term, ctx);

	if (!success) {
		return false;
	}

	if (u->op != OP_NONE) {

		const uint32_t dest = make_temp();

		enum TAC_OP op = TAC_OP_NONE;

		switch (u->op) {
			case OP_MINUS: op = TAC_OP_UNARY_MINUS; break;
			case OP_NOT: op = TAC_OP_UNARY_NOT; break;
			case OP_COMPLEMENT: op = TAC_OP_UNARY_BITWISE_NEG; break;

			default:
				fprintf(stderr, "error, op was none of supported TAC_OP_... values\n");
				fprintf(stderr, "op is %d\n", u->op);
				return false;
		}

		struct TAC* t = makeTACUnaryOp(u->super.line_num, dest, tacbuffer_last_dest(buffer), op);

		tacbuffer_append(buffer, t);
	}

	return true;
}

bool tac_unopterm_addr(struct TACBuffer* buffer, struct UnOpTerm* u, struct Ctx* ctx) {

	if (u->deref) {
		return tac_term(buffer, u->deref->term, ctx);
	}
	if (u->address_of) {
		fprintf(stderr, "%s: unsupported\n", __func__);
		assert(false);
	}
	if (u->term) {
		return tac_term_addr(buffer, u->term, ctx);
	}

	fprintf(stderr, "%s: unsupported\n", __func__);
	assert(false);
}
