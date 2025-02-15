#include <stdio.h>
#include <stdlib.h>

#include "gen_tac.h"
#include "tac/tacbuffer.h"

void tac_mdirect(struct TACBuffer* buffer, struct MDirect* m, struct Ctx* ctx) {

	tac_expr(buffer, m->expr, ctx);

	struct TAC* t = makeTACLoad(make_temp(), tacbuffer_last_dest(buffer), m->load_store_width);

	tacbuffer_append(buffer, t);
}
