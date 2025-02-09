#include <stdio.h>
#include <stdlib.h>

#include "gen_tac.h"
#include "tac/tacbuffer.h"

void tac_mdirect(struct TACBuffer* buffer, struct MDirect* m, struct Ctx* ctx) {

	tac_expr(buffer, m->expr, ctx);

	// TODO: for x86, this needs to be 8 bytes, not 2
	struct TAC* t = makeTACLoad(make_temp(), tacbuffer_last_dest(buffer), 2);

	tacbuffer_append(buffer, t);
}
