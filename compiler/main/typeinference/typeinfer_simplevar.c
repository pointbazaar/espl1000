#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "ast/util/str_ast.h"

#include "typeinference/typeinfer.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"
#include "tables/sst/sst.h"

struct Type* infer_type_simplevar_once(struct ST* st, struct SimpleVar* v) {

	if (lvst_contains(st->lvst, v->name)) {

		struct LVSTLine* line = lvst_get(st->lvst, v->name);

		return line->type;
	}

	if (v->count_indices != 0) { goto error; }

	if (sst_contains(st->sst, v->name)) {

		struct SSTLine* line2 = sst_get(st->sst, v->name);

		if (line2->type == NULL) { goto error; }

		return line2->type;
	}

error:
	printf("[Typeinference][Error]");
	printf("%s\n", str_simple_var(v));
	printf(" exiting.\n");
	exit(1);
	return NULL;
}

struct Type* infer_type_simplevar(struct ST* st, struct SimpleVar* v) {

	if (lvst_contains(st->lvst, v->name)) {

		struct LVSTLine* line = lvst_get(st->lvst, v->name);

		return unwrap_indices(line->type, v->count_indices);
	}

	if (v->count_indices != 0) { goto error; }

	if (sst_contains(st->sst, v->name)) {

		struct SSTLine* line2 = sst_get(st->sst, v->name);

		if (line2->type == NULL) { goto error; }

		return line2->type;
	}

error:
	printf("[Typeinference][Error]");
	printf("%s\n", str_simple_var(v));
	printf(" exiting.\n");
	exit(1);
	return NULL;
}
