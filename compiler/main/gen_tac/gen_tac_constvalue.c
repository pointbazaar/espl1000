#include <stdio.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"
#include "tables/enum/enum_table.h"

void tac_constvalue(struct TACBuffer* buffer, struct ConstValue* c) {

	tacbuffer_append(
	    buffer,
	    makeTACConst(c->super.line_num, make_temp(), int_value_from_const(c)));
}
