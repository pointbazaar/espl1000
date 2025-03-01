#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

bool tac_address_of(struct TACBuffer* buffer, struct AddressOf* ao, struct Ctx* ctx) {

	return tac_term_addr(buffer, ao->term, ctx);
}
