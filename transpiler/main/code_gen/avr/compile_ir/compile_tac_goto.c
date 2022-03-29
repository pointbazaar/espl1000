#include <stdio.h>

#include "code_gen/avr/tac/tac.h"
#include "code_gen/avr/compile_ir/compile_tac.h"

void compile_tac_goto(struct TAC* tac, FILE* fout){
    fprintf(fout, "rjmp L%d\n", tac->label_index);
}