#include <stdio.h>
#include <stdbool.h>

#include "ast/ast_declare.h"
#include "ast/ast.h"

#include "flags.h"

#include "cg_x86.h"

bool transpile_and_write_x86(char* asm_file_filename, struct AST* ast, struct Flags* flags){

    FILE* fout = fopen(asm_file_filename, "w");

    if(fout == NULL){
        return false;
    }

    printf("%d\n", ast->count_namespaces);
    printf("%d\n", flags->x86);

    //TODO

    puts("[Error] not implemented");
    return false;

    fclose(fout);

    return true;
}