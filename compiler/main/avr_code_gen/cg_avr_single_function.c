#include <stdio.h>

#include "cg_avr_basic_block.h"
#include "analyzer/lv/lv_analyzer.h"

#include "basic_block/basicblock.h"
#include "tac/tacbuffer.h"
#include "gen_tac/gen_tac.h"

#include "util/ctx.h"
#include "ast/ast.h"

#include "tables/symtable/symtable.h"
#include "cg_avr_single_function.h"

#include "../../cli/flags/flags.h"

void compile_and_write_avr_single_function(struct Method* m, struct Ctx* ctx, FILE* fout){

    struct TACBuffer* buffer = tacbuffer_ctor();

    tac_method(buffer, m, ctx);

    //print the TAC for debug
    if(flags_debug(ctx_flags(ctx)))
        tacbuffer_print(buffer);

    //create basic blocks from this TAC
    //basic blocks from the three address code
    //for each function, create a graph of basic blocks

	int nblocks;
    struct BasicBlock** graph = basicblock_create_graph(buffer, m->decl->name, &nblocks);
	struct BasicBlock* root = graph[0];

    //populate ctx->st->lvst
    lvst_clear(ctx_tables(ctx)->lvst);
    lvst_fill(m, ctx_tables(ctx));

    emit_asm_avr_basic_block(root, ctx, fout);
    
    //delete the basic block graph
    for(int i=0; i < nblocks; i++){
		basicblock_dtor(graph[i]);
	}
	free(graph);

    tacbuffer_dtor(buffer);
}
