#include <stdlib.h>

#include "gen_tac.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

void tac_retstmt(struct TACBuffer* buffer, struct RetStmt* r, struct Ctx* ctx) {

	if(r->return_value != NULL) {
		tac_expr(buffer, r->return_value, ctx);
	}

	struct TAC* t = makeTACReturn(tacbuffer_last_dest(buffer));

	tacbuffer_append(buffer, t);
}
