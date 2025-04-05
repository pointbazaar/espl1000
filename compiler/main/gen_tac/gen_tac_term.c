#include <stdio.h>

#include "tac/tacbuffer.h"

#include "gen_tac.h"

bool tac_term(struct TACBuffer* buffer, struct Term* t, struct Ctx* ctx) {

	switch (t->kind) {
		case TERM_KIND_CALL: tac_call(buffer, t->ptr.call_term, ctx); break;
		case TERM_KIND_EXPR: tac_expr(buffer, t->ptr.expr_term, ctx); break;
		case TERM_KIND_VAR: tac_variable(buffer, t->ptr.var_term, ctx); break;
		case TERM_KIND_STRINGCONST: return tac_const_data(buffer, t->ptr.stringconst_term, ctx);
		case TERM_KIND_CONSTVALUE: tac_constvalue(buffer, t->ptr.constvalue_term); break;
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
		case TERM_KIND_EXPR: tac_expr_addr(buffer, t->ptr.expr_term, ctx); break;
		case TERM_KIND_VAR: tac_variable_addr(buffer, t->ptr.var_term, ctx, &width); break;
		default:
			fprintf(stderr, "%s: unsupported: %d\n", __func__, t->kind);
			return false;
	}

	return true;
}
