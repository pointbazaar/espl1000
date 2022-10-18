#include <stdio.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

void tac_constvalue(struct TACBuffer* buffer, struct ConstValue* c){
    
    tacbuffer_append(
		buffer, 
		makeTACConst(make_temp(), int_value_from_const(c)), 
		true
	);
}
