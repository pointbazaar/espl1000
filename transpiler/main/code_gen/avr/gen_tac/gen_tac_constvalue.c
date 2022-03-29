#include <stdio.h>


#include "code_gen/avr/tac/tac.h"
#include "gen_tac.h"
#include "code_gen/avr/tacbuffer/tacbuffer.h"

void tac_constvalue(struct TACBuffer* buffer, struct ConstValue* c){
    struct TAC* t = makeTAC();
    t->kind = TAC_CONST_VALUE;
    sprintf(t->dest, "t%d", make_temp());

    t->const_value = int_value_from_const(c);

    tacbuffer_append(buffer, t, true);
}