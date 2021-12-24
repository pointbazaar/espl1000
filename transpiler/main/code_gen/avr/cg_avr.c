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


bool compile_and_write_avr(char* asm_file_filename, struct AST* ast, struct Flags* flags){

    //TODO
    if(flags->emit_headers){
        printf("-h not implemented for avr");
    }

    //TODO: convert AST into 3 address code with temporaries
    cap   = 10;
    count = 0;
    tac = malloc(sizeof(struct TAC*)*cap);

    //use recursive descent to make TAC
    for(size_t i = 0; i < ast->count_namespaces; i++){
        struct Namespace* ns = ast->namespaces[i];
        for(size_t j = 0; j < ns->count_methods; j++){
            struct Method* m = ns->methods[i];

            tac_method(m);
        }
    }

    //print the TAC for debug
    if(flags->debug){
        for(size_t i = 0; i < count; i++){
            print_tac(tac[i]);
        }
    }

    //TODO: do liveness analysis to assign registers
    //if we do not have enough registers, simply print an error and give up.
    //spilling will not be implemented (yet?)

    //TODO
    FILE* out_file = fopen(asm_file_filename, "w");
    if(out_file == NULL){
        //TODO
    }

    fprintf(out_file, "%s", "TODO");

    return false;
}