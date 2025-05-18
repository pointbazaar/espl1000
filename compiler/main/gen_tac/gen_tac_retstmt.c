#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

bool tac_retstmt(struct TACBuffer* buffer, struct RetStmt* r, struct Ctx* ctx) {

	if (r->return_value != NULL) {
		if (!tac_expr(buffer, r->return_value, ctx)) {
			return false;
		}
	}

	struct TAC* t = makeTACReturn(r->super.line_num, tacbuffer_last_dest(buffer));

	tacbuffer_append(buffer, t);

	return true;
}
