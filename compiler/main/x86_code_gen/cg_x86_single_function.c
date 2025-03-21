#include <stdlib.h>
#include <assert.h>

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

#include "ibuffer/ibuffer.h"

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

	//populate ctx->st->lvst
	lvst_clear(st->lvst);
	lvst_fill(m, st);

	//print the LVST for debug
	if (flags_debug(ctx_flags(ctx)))
		lvst_print(st->lvst);

	tac_method(buffer, m, ctx);

	if (flags_debug(ctx_flags(ctx))) {

		printf("\nTACBuffer before optimization:\n");
		tacbuffer_print(buffer, st->sst, st->lvst);
	}

	success = tacbuffer_optimize_reorder(buffer, flags_debug(ctx_flags(ctx)));

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

	char* current_function_name = m->decl->name;
	assert(current_function_name != NULL);

	if (flags_debug(ctx_flags(ctx))) {

		printf("RAT for function '%s'\n", m->decl->name);
		rat_print(rat);
	}

	success = emit_asm_x86_basic_block(root, ctx, ibu, rat, current_function_name);

	// TODO: propagate this error
	assert(success);
exit:
	//delete the basic block graph
	for (int i = 0; i < nblocks; i++) {
		basicblock_dtor(graph[i]);
	}
	free(graph);
exit_tacbuffer:

	tacbuffer_dtor(buffer);

	return success;
}
