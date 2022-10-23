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


void emit_create_stack_frame(uint32_t stack_frame_size, FILE* fout){

    //push onto the stack to create the stack frame
    for(size_t k=0; k < stack_frame_size; k++){
        fprintf(fout, "push r0  ;create frame\n"); //it is irrelevant what we push here
    }
    fprintf(fout, "\n");
}

void compile_and_write_avr_single_function(struct Method* m, struct Ctx* ctx, FILE* fout){
    struct TACBuffer* buffer = tacbuffer_ctor();

    //first the label of the function
    tacbuffer_append(buffer, makeTACLabel2(m->decl->name));

    tac_method(buffer, m);

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

    emit_create_stack_frame(lvst_stack_frame_size_avr(ctx_tables(ctx)->lvst), fout);

    //now load X as our base pointer for the stack frame
    fprintf(fout, "in r28, SPL  ;Y is base ptr\n");
    fprintf(fout, "in r29, SPH  ;Y is base ptr\n\n");

    emit_asm_avr_basic_block(root, ctx, fout);
    
    //delete the basic block graph
    for(int i=0; i < nblocks; i++){
		basicblock_dtor(graph[i]);
	}
	free(graph);

    tacbuffer_dtor(buffer);
}
