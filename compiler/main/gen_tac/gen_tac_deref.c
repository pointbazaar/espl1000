#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

bool tac_deref(struct TACBuffer* buffer, struct Deref* d, struct Ctx* ctx) {

	tac_term(buffer, d->term, ctx);

	const uint32_t dest = make_temp();

	struct TAC* last = tacbuffer_get_last(buffer);

	const uint32_t width = tac_load_store_width(last);

	tacbuffer_append(buffer, makeTACLoad(dest, tacbuffer_last_dest(buffer), width));

	return true;
}
