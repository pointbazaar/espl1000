#include <stdio.h>

#include "code_gen/avr/tacbuffer/tacbuffer.h"
#include "code_gen/avr/tac/tac.h"
#include "code_gen/avr/compile_ir/compile_tac.h"

void compile_tac_label(struct TAC* tac, FILE* fout){
    if(strcmp(tac->dest, "") == 0)
        fprintf(fout, "L%d:\n", tac->label_index);
    else
        fprintf(fout, "%s:\n", tac->dest);
}