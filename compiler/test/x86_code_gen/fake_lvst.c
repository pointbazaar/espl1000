#define _GNU_SOURCE

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "util/fill_tables.h"
#include "tables/symtable/symtable.h"

#include "util/ctx.h"

struct Type* fake_uint64_type() {

	struct Type* type = calloc(1, sizeof(struct Type));

	struct BasicType* basicType = calloc(1, sizeof(struct BasicType));
	type->basic_type = basicType;

	struct SimpleType* simpleType = calloc(1, sizeof(struct SimpleType));
	basicType->simple_type = simpleType;

	struct PrimitiveType* pt = calloc(1, sizeof(struct PrimitiveType));
	simpleType->primitive_type = pt;
	pt->is_int_type = true;
	pt->int_type = UINT64;

	return type;
}

static void sd_uc_fake_lvst_line(struct Ctx* ctx, struct LVST* lvst, int i, bool is_arg) {

	struct LVSTLine* line = malloc(sizeof(struct LVSTLine));
	assert(line != NULL);

	asprintf(&(line->name), "fake variable %d", i);
	struct Type* type = fake_uint64_type();

	line->type = type;
	line->is_arg = is_arg;
	line->read_only = false;

	lvst_add(lvst, line);

	// register the type so it can be freed later
	st_register_inferred_type(ctx_tables(ctx), type);
}

void sd_uc_fake_lvst(struct Ctx* ctx, size_t fake_lvst_size, size_t stackframe_nargs) {
	struct ST* st = ctx_tables(ctx);
	assert(st != NULL);

	struct LVST* lvst = st->lvst;
	assert(lvst != NULL);

	size_t nlocals = fake_lvst_size - stackframe_nargs;

	for (size_t i = 0; i < nlocals; i++) {
		sd_uc_fake_lvst_line(ctx, lvst, i, false);
	}
	for (size_t i = 0; i < stackframe_nargs; i++) {
		sd_uc_fake_lvst_line(ctx, lvst, i, true);
	}
}
