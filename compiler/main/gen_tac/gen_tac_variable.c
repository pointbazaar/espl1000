#include <stdio.h>
#include <stdlib.h>

#include "cli/flags/flags.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "helper_gen_tac_derefll.h"
#include "derefll/derefll.h"

#include "gen_tac.h"

void tac_variable(struct TACBuffer* buf, struct Variable* v, struct Ctx* ctx) {

	tac_variable_addr(buf, v, ctx);

	const uint32_t tlast = tacbuffer_last_dest(buf);

	// TODO: do not assume the variable has 2 bytes
	// on avr, it can only be 1 byte
	// on x86 it can be up to 8
	tacbuffer_append(buf, makeTACLoad(make_temp(), tlast, 2));
}

void tac_variable_addr(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx) {

	if (flags_debug(ctx_flags(ctx))) {
		printf("[debug] %s\n", __func__);
	}

	struct DerefLL* dll = derefll_ctor_variable(v, ctx);

	derefll_annotate_types(dll, ctx, NULL);

	//now iterate over the linked list and emit the correct TACs along the way

	struct DerefLL* current = dll;
	struct Type* prev_type = NULL;

	while (true) {

		tac_derefll_single(buffer, current, prev_type, ctx);

		prev_type = current->type;
		current = current->next;

		if (current == NULL) break;
	}

	derefll_dtor(dll);
}
