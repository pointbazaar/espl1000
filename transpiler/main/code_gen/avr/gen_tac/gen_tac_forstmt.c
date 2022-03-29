#include <stdio.h>
#include <stdlib.h>

#include "code_gen/avr/tac/tac.h"
#include "code_gen/avr/tacbuffer/tacbuffer.h"
#include "gen_tac.h"

void tac_forstmt(struct TACBuffer* buffer, struct ForStmt* f){
    //TODO: we should convert for loops into while loops before 3 address code
    printf("not implemented: ForStmt, %s\n", f->index_name);
    exit(1+buffer->count);
}