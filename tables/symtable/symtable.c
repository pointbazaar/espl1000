#include <stdlib.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "tables/lvst/lvst.h"
#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "symtable.h"

struct ST* st_ctor() {

	struct ST* st = make(ST);

	st->inferred_types_capacity = 100;
	st->inferred_types_count = 0;

	const uint32_t nbytes = sizeof(struct Type*) * st->inferred_types_capacity;

	st->inferred_types = exit_malloc(nbytes);

	st->sst = sst_ctor();
	st->stst = stst_ctor();
	st->lvst = lvst_ctor();

	return st;
}

void st_free(struct ST* st) {

	if (st->sst != NULL) { sst_free(st->sst); }
	if (st->lvst != NULL) { lvst_free(st->lvst); }
	if (st->stst != NULL) { stst_free(st->stst); }

	for (int i = 0; i < st->inferred_types_count; i++) {
		free_type(st->inferred_types[i]);
	}

	free(st->inferred_types);

	free(st);
}

void st_register_inferred_type(struct ST* st, struct Type* t) {

	if (st->inferred_types_count >= st->inferred_types_capacity) {

		//increase capacity
		st->inferred_types_capacity *= 2;

		uint64_t nbytes = (st->inferred_types_capacity) * sizeof(struct Type*);
		st->inferred_types = realloc(st->inferred_types, nbytes);
	}

	st->inferred_types[st->inferred_types_count] = t;
	st->inferred_types_count++;
}
