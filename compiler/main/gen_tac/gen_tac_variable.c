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

	uint8_t last_type_width;

	tac_variable_addr(buf, v, ctx, &last_type_width);

	const uint32_t tlast = tacbuffer_last_dest(buf);

	// in case of function pointer, we do not need to dereference again
	{
		struct TAC* last = tacbuffer_get_last(buf);
		if (tac_kind(last) == TAC_LOAD_FUNCTION_PTR) {
			return;
		}
	}

	tacbuffer_append(buf, makeTACLoad(v->super.line_num, make_temp(), tlast, last_type_width));
}

void tac_variable_addr(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx, uint8_t* last_type_width) {

	if (flags_debug(ctx_flags(ctx))) {
		printf("[debug] %s\n", __func__);
	}

	const bool x86 = flags_x86(ctx_flags(ctx));

	struct DerefLL* dll = derefll_ctor_variable(v, ctx);

	derefll_annotate_types(dll, ctx, NULL);

	//now iterate over the linked list and emit the correct TACs along the way

	struct DerefLL* current = dll;
	struct Type* prev_type = NULL;

	while (current != NULL) {

		tac_derefll_single(buffer, current, prev_type, ctx);

		if (last_type_width && current->type) {
			*last_type_width = lvst_sizeof_type(current->type, x86);
		}

		prev_type = current->type;
		current = current->next;
	}

	derefll_dtor(dll);
}
