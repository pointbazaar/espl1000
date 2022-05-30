#include <stdio.h>

#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_nop(FILE* fout){
    fprintf(fout, "nop\n");
}