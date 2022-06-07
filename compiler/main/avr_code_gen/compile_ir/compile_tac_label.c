#include <stdio.h>

#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

void compile_tac_label(struct TAC* tac, FILE* fout){
    if(strcmp(tac->dest, "") == 0)
        fprintf(fout, "L%d:\n", tac->label_index);
    else
        fprintf(fout, "%s:\n", tac->dest);
}