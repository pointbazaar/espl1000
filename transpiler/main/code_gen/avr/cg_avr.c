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

static void emit_asm_avr(struct BasicBlock* block){

    //TODO: do liveness analysis to assign registers
    //if we do not have enough registers, simply print an error and give up.
    //spilling will not be implemented (yet?)

    //simplest naive approach (first iteration):
    //simply get a new register for each temporary
    //the mapping tx -> ry can be saved in an array
    //TODO: use better approach

    bool registers_used[32];
    memset(registers_used, false, sizeof(bool)*32);

    uint8_t register_map[32];
    uint8_t register_map_size = 0;

    basicblock_assign_registers(block, register_map, &register_map_size, registers_used);

    //TODO: emit
}

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

            //create basic blocks from this TAC
            //basic blocks from the three address code
            //for each function, create a graph of basic blocks

            struct BasicBlock* root = basicblock_create_graph(buffer, m->decl->name);

            emit_asm_avr(root);

            tacbuffer_dtor(buffer);
        }
    }

    //TODO
    //we have to use the parameter, else compile error
    printf("%s\n", asm_file_filename);

    return false;
}