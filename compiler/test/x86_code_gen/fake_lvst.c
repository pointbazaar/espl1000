#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "util/fill_tables.h"
#include "tables/symtable/symtable.h"

#include "util/ctx.h"

static void sd_uc_fake_lvst_line(struct Ctx* ctx, struct LVST* lvst, int i) {

	struct LVSTLine* line = malloc(sizeof(struct LVSTLine));
	assert(line != NULL);

	asprintf(&(line->name), "fake variable %d", i);
	struct Type* type = calloc(1, sizeof(struct Type));

	struct BasicType* basicType = calloc(1, sizeof(struct BasicType));
	type->basic_type = basicType;

	struct SimpleType* simpleType = calloc(1, sizeof(struct SimpleType));
	basicType->simple_type = simpleType;

	struct PrimitiveType* pt = calloc(1, sizeof(struct PrimitiveType));
	simpleType->primitive_type = pt;
	pt->is_int_type = true;
	pt->int_type = UINT64;

	line->type = type;
	line->first_occur = NULL;
	line->is_arg = false;
	line->read_only = false;

	lvst_add(lvst, line);
}

void sd_uc_fake_lvst(struct Ctx* ctx, size_t fake_lvst_size) {
	struct ST* st = ctx_tables(ctx);
	assert(st != NULL);

	struct LVST* lvst = st->lvst;
	assert(lvst != NULL);

	for (int i = 0; i < fake_lvst_size; i++) {
		sd_uc_fake_lvst_line(ctx, lvst, i);
	}
}
