#include <stdio.h>

#include "code_gen/avr/tac/tac.h"

void compile_tac_call(struct TAC* tac, FILE* fout){
    fprintf(fout, "call %s\n", tac->arg1);
}