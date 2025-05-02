#define _GNU_SOURCE

#include <stdlib.h>

#include "cg_x86_basic_block.h"
#include "analyzer/lv/lv_analyzer.h"

#include "basic_block/basicblock.h"
#include "tac/tacbuffer.h"
#include "gen_tac/gen_tac.h"
#include "rat/rat.h"

#include "util/ctx.h"
#include "rat/rat.h"
#include "ast/ast.h"
#include "ast/ast_declare.h"

#include "ibuffer/ibuffer_avr.h"

#include "tables/symtable/symtable.h"
#include "cg_x86_single_function.h"
#include "allocate_registers_x86.h"

#include "cli/flags/flags.h"
#include "liveness/liveness.h"

bool compile_and_write_x86_single_function(struct Method* m, struct Ctx* ctx, struct IBuffer* ibu) {

	bool success = true;

	if (has_annotation(m->super.annotations, ANNOT_EXTERN) || has_annotation(m->super.annotations, ANNOT_SYSCALL)) {
		return true;
	}

	struct TACBuffer* buffer = tacbuffer_ctor();
	struct ST* st = ctx_tables(ctx);

	char* current_function_name = m->decl->name;

	if (current_function_name == NULL) {
		success = false;
		goto exit_name;
	}

	struct SSTLine* line = sst_get(st->sst, current_function_name);

	if (!line) {
		success = false;
		goto exit_name;
	}

	if (line->dead != DEAD_ISLIVE) {

		if (flags_debug_dead(ctx_flags(ctx))) {
			printf("[debug][dead-code]: skip %s\n", current_function_name);
		}

		return true;
	}

	//populate ctx->st->lvst
	lvst_clear(st->lvst);
	lvst_fill(m, st);

	//print the LVST for debug
	if (flags_debug(ctx_flags(ctx))) {
		char* lvst_filename = NULL;
		asprintf(&lvst_filename, "%s.lvst", current_function_name);

		if (!lvst_filename) {
			return false;
		}

		lvst_print_filename(st->lvst, lvst_filename);

		free(lvst_filename);
	}

	if (!tac_method(buffer, m, ctx)) {
		return false;
	}

	if (flags_debug(ctx_flags(ctx))) {

		printf("\nTACBuffer before optimization:\n");
		tacbuffer_print(buffer, st->sst, st->lvst);
	}

	success = tacbuffer_optimize_reorder(buffer, ctx_flags(ctx));

	if (!success) {
		goto exit_tacbuffer;
	}

	if (flags_debug(ctx_flags(ctx))) {

		printf("\nTACBuffer after optimization:\n");
		tacbuffer_print(buffer, st->sst, st->lvst);
	}

	//create basic blocks from this TAC
	//basic blocks from the three address code
	//for each function, create a graph of basic blocks

	int nblocks;
	struct BasicBlock** graph = basicblock_create_graph(buffer, m->decl->name, &nblocks, ctx);
	struct BasicBlock* root = graph[0];

	struct Liveness* live = liveness_calc(graph, nblocks);

	struct RAT* rat = rat_ctor(RAT_ARCH_X86, liveness_ntemps(live));

	success = allocate_registers_basicblocks(graph, nblocks, rat, st, live);

	if (!success) {
		goto exit;
	}

	if (flags_debug(ctx_flags(ctx))) {

		char* rat_filename = NULL;
		asprintf(&rat_filename, "%s.rat", current_function_name);

		if (!rat_filename) {
			success = false;
			goto exit;
		}

		FILE* fout_rat = fopen(rat_filename, "w");
		if (!fout_rat) {
			success = false;
			free(rat_filename);
			goto exit;
		}

		rat_print(rat, fout_rat);
		printf("RAT for function '%s' dumped to %s\n", m->decl->name, rat_filename);

		free(rat_filename);
		fclose(fout_rat);
	}

	success = emit_asm_x86_basic_block(root, ctx, ibu, rat, current_function_name);

exit:
	//delete the basic block graph
	for (int i = 0; i < nblocks; i++) {
		basicblock_dtor(graph[i]);
	}
	free(graph);
exit_tacbuffer:

	tacbuffer_dtor(buffer);
exit_name:
	return success;
}
