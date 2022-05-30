#include <stdio.h>

#include "avr_code_gen/tac/tac.h"
#include "avr_code_gen/tacbuffer/tacbuffer.h"
#include "gen_tac.h"

void tac_constvalue(struct TACBuffer* buffer, struct ConstValue* c){
    struct TAC* t = makeTAC();
    t->kind = TAC_CONST_VALUE;
    sprintf(t->dest, "t%d", make_temp());

    t->const_value = int_value_from_const(c);

    tacbuffer_append(buffer, t, true);
}