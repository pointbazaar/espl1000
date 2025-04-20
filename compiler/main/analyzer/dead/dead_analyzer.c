#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "ast/visitor/visitor.h"

#include "cli/flags/flags.h"
#include "tables/cc/cc.h"
#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "dead.h"
#include "dead_analyzer.h"

static void mark_live(struct Ctx* ctx, char* name);

static void set_all(struct SST* sst, enum DEAD dead);

/* BUG: when passing a function pointer
 * to another function, the function pointed to
 * is marked as dead, as there are no calls to it.
 * but it is not dead. we could assume that all
 * functions which are referred to as function
 * pointers could be live. so every simple_var
 * in the program must be analyzed, if it is the name
 * of a function. that function may then be marked
 * as live, as it could be live.
 */

static void myvisitor_dead(void* node, enum NODE_TYPE type, void* arg);

bool analyze_dead_code(struct Ctx* ctx, struct AST* ast) {

	struct Flags* flags = ctx_flags(ctx);

	if (flags_debug_dead(flags)) {
		printf("[debug][dead-code] start analysis...\n");
	}

	struct ST* st = ctx_tables(ctx);
	struct SST* sst = st->sst;

	//set all functions to live
	set_all(sst, DEAD_ISLIVE);

	if (!sst_contains(sst, "main")) { return true; }

	set_all(sst, DEAD_UNKNOWN);

	mark_live(ctx, "main");

	if (flags_debug_dead(flags)) {
		printf("[debug][dead-code] visiting live functions\n");
	}

	for (int i = 0; i < ast->count_namespaces; i++) {
		struct Namespace* ns = ast->namespaces[i];
		for (size_t j = 0; j < ns->count_methods; j++) {
			struct Method* m = ns->methods[j];

			if (!m || !m->decl || !m->decl->name) {
				return false;
			}

			const char* name = m->decl->name;

			struct SSTLine* line = sst_get(sst, name);

			if (line->dead != DEAD_ISLIVE) {
				continue;
			}

			if (!visit_method(m, myvisitor_dead, ctx)) {
				return false;
			}
		}
	}

	if (flags_debug_dead(flags)) {
		printf("[debug][dead-code] end analysis...\n");
	}

	return true;
}

static void mark_live(struct Ctx* ctx, char* name) {

	struct SST* sst = ctx_tables(ctx)->sst;
	struct SSTLine* line = sst_get(sst, name);

	if (line->dead != DEAD_UNKNOWN) { return; }

	if (flags_debug_dead(ctx_flags(ctx))) {
		printf("[debug][dead-code]: LIVE FUNCTION: %s\n", line->name);
	}

	line->dead = DEAD_ISLIVE;

	struct CCNode* callee = line->cc->callees;

	while (callee != NULL) {

		mark_live(ctx, cc_name(callee));

		callee = cc_next(callee);
	}
}

static void set_all(struct SST* sst, enum DEAD dead) {

	for (uint32_t i = 0; i < sst_size(sst); i++) {

		struct SSTLine* line = sst_at(sst, i);

		line->dead = dead;
	}
}

static void myvisitor_dead(void* node, enum NODE_TYPE type, void* arg) {

	/* if we are dealing with a variable that
	 * is a function pointer, the function
	 * pointed to is assumed to be live.
	 */

	struct Ctx* ctx = (struct Ctx*)arg;
	struct SST* mysst = ctx_tables(ctx)->sst;

	if (type == NODE_SIMPLEVAR) {

		struct SimpleVar* v = (struct SimpleVar*)node;

		if (sst_contains(mysst, v->name)) {

			mark_live(ctx, v->name);
		}
	}
}
