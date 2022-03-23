#include <stdbool.h>

#include <stdio.h>
#include <tables/symtable/symtable.h>
#include <stdlib.h>

#include "ast/ast_declare.h"
#include "ast/ast.h"

#include "flags.h"
#include "util/ctx.h"

#include "cg_avr.h"
#include "tac.h"
#include "tacbuffer.h"
#include "basicblock.h"
#include "analyzer/lv/lv_analyzer.h"
#include "cg_avr_basic_block.h"

bool compile_and_write_avr(char* asm_file_filename, struct AST* ast, struct Flags* flags, struct Ctx* ctx){

    if(flags->emit_headers){
        printf("-h not implemented for avr");
    }

    FILE* fout = fopen(asm_file_filename, "w");
    if(fout == NULL){
        printf("error opening output file\n");
        exit(1);
    }

    fprintf(fout, ".device ATmega328P\n");

    //convert AST into 3 address code with temporaries

    //use recursive descent to make TAC
    for(size_t i = 0; i < ast->count_namespaces; i++){
        struct Namespace* ns = ast->namespaces[i];

        for(size_t j = 0; j < ns->count_methods; j++){
            struct Method* m = ns->methods[j];

            struct TACBuffer* buffer = tacbuffer_ctor();

            tac_method(buffer, m);

            //print the TAC for debug
            if(flags->debug)
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
            emit_asm_avr_basic_block(root, ctx->tables, flags, fout);

            tacbuffer_dtor(buffer);
        }
    }

    fclose(fout);

    return true;
}