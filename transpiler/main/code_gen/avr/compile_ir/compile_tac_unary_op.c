#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "code_gen/avr/rat/rat.h"
#include "code_gen/avr/tac/tac.h"
#include "code_gen/avr/compile_ir/compile_tac.h"


void compile_tac_unary_op(struct RAT* rat, struct TAC* tac, FILE* fout){

    printf("%d\n", rat->capacity);
    printf("%d\n", tac->kind);
    printf("%d\n", fout->_fileno);
    printf("currently unsupported: TAC_UNARY_OP\n");
    exit(1);
}
