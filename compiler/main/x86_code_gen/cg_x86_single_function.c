#include <stdlib.h>

#include "cg_x86_basic_block.h"
#include "analyzer/lv/lv_analyzer.h"

#include "basic_block/basicblock.h"
#include "tac/tacbuffer.h"
#include "gen_tac/gen_tac.h"

#include "util/ctx.h"
#include "ast/ast.h"

#include "ibuffer/ibuffer.h"

#include "tables/symtable/symtable.h"
#include "cg_x86_single_function.h"

#include "../../cli/flags/flags.h"

void compile_and_write_x86_single_function(struct Method* m, struct Ctx* ctx, struct IBuffer* ibu) {

	struct TACBuffer* buffer = tacbuffer_ctor();

	//populate ctx->st->lvst
	lvst_clear(ctx_tables(ctx)->lvst);
	lvst_fill(m, ctx_tables(ctx));

	//print the LVST for debug
	if (flags_debug(ctx_flags(ctx)))
		lvst_print(ctx_tables(ctx)->lvst);

	tac_method(buffer, m, ctx);

	//print the TAC for debug
	if (flags_debug(ctx_flags(ctx)))
		tacbuffer_print(buffer, ctx);

	//create basic blocks from this TAC
	//basic blocks from the three address code
	//for each function, create a graph of basic blocks

	int nblocks;
	struct BasicBlock** graph = basicblock_create_graph(buffer, m->decl->name, &nblocks, ctx);
	struct BasicBlock* root = graph[0];

	struct RAT* rat = rat_ctor(RAT_ARCH_X86);
	for (int i = 0; i < nblocks; i++) {
		allocate_registers(graph[i]->buffer, rat, ctx_tables(ctx));
	}

	emit_asm_x86_basic_block(root, ctx, ibu, rat);

	//delete the basic block graph
	for (int i = 0; i < nblocks; i++) {
		basicblock_dtor(graph[i]);
	}
	free(graph);

	tacbuffer_dtor(buffer);
}
