#include <stdbool.h>

#include <stdio.h>
#include <tables/symtable/symtable.h>
#include <stdlib.h>

#include "ast/ast_declare.h"
#include "ast/ast.h"

#include "flags.h"
#include "util/ctx.h"

#include "cg_avr.h"
#include "code_gen/avr/tac/tac.h"
#include "code_gen/avr/tacbuffer/tacbuffer.h"
#include "code_gen/avr/basic_block/basicblock.h"
#include "analyzer/lv/lv_analyzer.h"
#include "cg_avr_basic_block.h"
#include "cg_avr_single_function.h"

static void emit_setup_stack_pointer_avr(FILE* fout){
    //initialize Stack Pointer
    fprintf(fout, "ldi r16, high(RAMEND)\n");
    fprintf(fout, "out SPH, r16\n");
    fprintf(fout, "ldi r16, low(RAMEND)\n");
    fprintf(fout, "out SPL, r16\n");

    fprintf(fout, "\n\n");
}

bool compile_and_write_avr(char* asm_file_filename, struct AST* ast, struct Ctx* ctx){

    if(ctx->flags->emit_headers){
        printf("-h not implemented for avr");
    }

    FILE* fout = fopen(asm_file_filename, "w");
    if(fout == NULL){
        printf("error opening output file\n");
        exit(1);
    }

    //in /usr/share/avra

    //in this file a comment must be shortened, otherwise avra will give an error
    fprintf(fout, ".INCLUDE \"/usr/share/avra/m32def.inc\"\n");
    //fprintf(fout, ".DEVICE ATmega328P\n");

    emit_setup_stack_pointer_avr(fout);

    //create the endloop such that the mcu does
    //not start over again after main
    fprintf(fout, "call main\n");
    fprintf(fout, "endloop:\n");
    fprintf(fout, "rjmp endloop\n"); //endloop

    //convert AST into 3 address code with temporaries, use recursive descent to make TAC
    for(size_t i = 0; i < ast->count_namespaces; i++){
        struct Namespace* ns = ast->namespaces[i];

        for(size_t j = 0; j < ns->count_methods; j++){
            struct Method* m = ns->methods[j];

            compile_and_write_avr_single_function(m, ctx, fout);
        }
    }

    fclose(fout);

    return true;
}