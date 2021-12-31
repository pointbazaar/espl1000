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


bool compile_and_write_avr(char* asm_file_filename, struct AST* ast, struct Flags* flags){

    //TODO
    if(flags->emit_headers){
        printf("-h not implemented for avr");
    }

    //convert AST into 3 address code with temporaries

    //use recursive descent to make TAC
    for(size_t i = 0; i < ast->count_namespaces; i++){
        struct Namespace* ns = ast->namespaces[i];

        for(size_t j = 0; j < ns->count_methods; j++){
            struct Method* m = ns->methods[j];

            struct TACBuffer* buffer = tacbuffer_ctor();

            tac_method(buffer, m);

            //print the TAC for debug
            if(flags->debug){
                tacbuffer_print(buffer);
            }

            //TODO: create basic blocks from this TAC
            //basic blocks from the three address code
            //for each function, create a graph of basic blocks

            /*struct BasicBlock* root = */basicblock_create_graph(buffer, m->decl->name);

            tacbuffer_dtor(buffer);
        }
    }

    //TODO: do liveness analysis to assign registers
    //if we do not have enough registers, simply print an error and give up.
    //spilling will not be implemented (yet?)

    //TODO
    //we have to use the parameter, else compile error
    printf("%s\n", asm_file_filename);

    return false;
}