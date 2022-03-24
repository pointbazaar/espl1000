#include <stdio.h>

#include "cg_avr_basic_block.h"
#include "analyzer/lv/lv_analyzer.h"
#include "basicblock.h"
#include "tacbuffer.h"
#include "tac.h"
#include "util/ctx.h"
#include "flags.h"
#include "ast/ast.h"
#include "ast/ast_declare.h"

#include "tables/symtable/symtable.h"

#include "cg_avr_single_function.h"

void compile_and_write_avr_single_function(struct Method* m, struct Ctx* ctx, FILE* fout){
    struct TACBuffer* buffer = tacbuffer_ctor();

    tac_method(buffer, m);

    //print the TAC for debug
    if(ctx->flags->debug)
        tacbuffer_print(buffer);

    //create basic blocks from this TAC
    //basic blocks from the three address code
    //for each function, create a graph of basic blocks

    struct BasicBlock* root = basicblock_create_graph(buffer, m->decl->name);

    //populate ctx->st->lvst
    lvst_clear(ctx->tables->lvst);
    lvst_fill(m, ctx->tables);

    //emit label for the function
    fprintf(fout, "%s:\n",m->decl->name);

    //push onto the stack to create the stack frame
    for(size_t k=0; k < lvst_stack_frame_size_avr(ctx->tables->lvst); k++){
        fprintf(fout, "push r0  ;create frame\n\n"); //it is irrelevant what we push here
    }

    //now load X as our base pointer for the stack frame
    fprintf(fout, "in r26, SPL  ;X is base ptr\n");
    fprintf(fout, "in r27, SPH  ;X is base ptr\n\n");

    emit_asm_avr_basic_block(root, ctx, fout);

    tacbuffer_dtor(buffer);
}