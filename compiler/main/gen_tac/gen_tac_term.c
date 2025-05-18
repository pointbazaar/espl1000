#include <stdio.h>
#include <assert.h>

#include "tac/tacbuffer.h"
#include "tables/symtable/symtable.h"
#include "tables/enum/enum_table.h"

#include "gen_tac.h"

static bool tac_term_enum_value(struct TACBuffer* buffer, struct Term* t, struct Ctx* ctx) {

	const struct ST* st = ctx_tables(ctx);
	assert(st);

	char* name = t->ptr.enum_value_term;
	assert(name);

	int64_t value = 0;
	const bool success = enum_table_lookup(st->enum_table, name, &value);

	if (!success) {
		fprintf(stderr, "could not find value for '%s' in enum table\n", name);

		enum_table_print(st->enum_table);
		return false;
	}

	tacbuffer_append(buffer, makeTACConst(t->super.line_num, make_temp(), value));

	return true;
}

bool tac_term(struct TACBuffer* buffer, struct Term* t, struct Ctx* ctx) {

	assert(buffer);
	assert(t);
	assert(ctx);

	switch (t->kind) {
		case TERM_KIND_CALL: tac_call(buffer, t->ptr.call_term, ctx); break;
		case TERM_KIND_EXPR: tac_expr(buffer, t->ptr.expr_term, ctx); break;
		case TERM_KIND_VAR: tac_variable(buffer, t->ptr.var_term, ctx); break;
		case TERM_KIND_STRINGCONST: return tac_const_data(buffer, t->ptr.stringconst_term, ctx);
		case TERM_KIND_CONSTVALUE: tac_constvalue(buffer, t->ptr.constvalue_term); break;
		case TERM_KIND_ENUM_VALUE:
			return tac_term_enum_value(buffer, t, ctx);
			break;
		default:
			fprintf(stderr, "%s: unsupported: %d\n", __func__, t->kind);
			return false;
	}

	return true;
}

bool tac_term_addr(struct TACBuffer* buffer, struct Term* t, struct Ctx* ctx) {

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
