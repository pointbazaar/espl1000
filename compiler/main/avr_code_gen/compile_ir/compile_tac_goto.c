#include <stdio.h>

#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_goto(struct TAC* tac, FILE* fout){
    fprintf(fout, "rjmp L%d\n", tac->label_index);
}