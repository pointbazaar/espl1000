#include <stdio.h>

#include "tac/tacbuffer.h"

#include "gen_tac.h"

bool tac_term(struct TACBuffer* buffer, struct Term* t, struct Ctx* ctx) {

	switch (t->kind) {
		case 4: tac_call(buffer, t->ptr.m4, ctx); break;
		case 5: tac_expr(buffer, t->ptr.m5, ctx); break;
		case 6: tac_variable(buffer, t->ptr.m6, ctx); break;
		case 8: return tac_const_data(buffer, t->ptr.m8, ctx);
		case 11:
			fprintf(stderr, "Fatal Error. Lambdas should not exist at this stage.\n");
			return false;
			//lambdas should not exist anymore at this stage,
			//having been converted into named functions
		case 12: tac_constvalue(buffer, t->ptr.m12); break;
		default:
			fprintf(stderr, "%s: unsupported: %d\n", __func__, t->kind);
			return false;
	}

	return true;
}

bool tac_term_addr(struct TACBuffer* buffer, struct Term* t, struct Ctx* ctx) {

	struct TAC* last = tacbuffer_get_last(buffer);
	uint8_t width;

	switch (t->kind) {
		case 5: tac_expr_addr(buffer, t->ptr.m5, ctx); break;
		case 6: tac_variable_addr(buffer, t->ptr.m6, ctx, &width); break;
		default:
			fprintf(stderr, "%s: unsupported: %d\n", __func__, t->kind);
			return false;
	}

	return true;
}
