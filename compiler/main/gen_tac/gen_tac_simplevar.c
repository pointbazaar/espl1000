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

void tac_simplevar(struct TACBuffer* buffer, struct SimpleVar* v, struct Ctx* ctx) {

	const bool x86 = flags_x86(ctx_flags(ctx));

	tac_simplevar_addr(buffer, v, ctx);

	uint32_t tlast = tacbuffer_last_dest(buffer);

	struct LVST* lvst = ctx_tables(ctx)->lvst;
	const uint8_t simplevar_width = lvst_sizeof_var(lvst, v->name, x86);

	tacbuffer_append(buffer, makeTACLoad(v->super.line_num, make_temp(), tlast, simplevar_width));
}

void tac_simplevar_addr(struct TACBuffer* buffer, struct SimpleVar* sv, struct Ctx* ctx) {

	struct DerefLL* dll = derefll_ctor_simplevar(sv, ctx);

	derefll_annotate_types(dll, ctx, NULL);

	//now iterate over the linked list and emit the correct TACs along the way

	struct DerefLL* current = dll;
	struct Type* prev_type = NULL;

	while (current->next != NULL || current == dll) {

		tac_derefll_single(buffer, current, prev_type, ctx);

		prev_type = current->type;
		current = current->next;
	}

	derefll_dtor(dll);
}
