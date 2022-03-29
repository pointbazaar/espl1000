#include <stdio.h>

#include "code_gen/avr/compile_ir/compile_tac.h"

void compile_tac_nop(FILE* fout){
    fprintf(fout, "nop\n");
}