#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "helper_gen_tac_derefll.h"
#include "derefll/derefll.h"

#include "gen_tac.h"

void tac_variable(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx) {

	tac_variable_addr(buffer, v, ctx);

	uint32_t tlast = tacbuffer_last_dest(buffer);

	tacbuffer_append(buffer, makeTACLoad(make_temp(), tlast));
}

void tac_variable_addr(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx) {

	struct DerefLL* dll = derefll_ctor_variable(v);

	derefll_annotate_types(dll, ctx, NULL);

	//now iterate over the linked list and emit the correct TACs along the way

	struct DerefLL* current = dll;
	struct Type* prev_type = NULL;

	while (current == dll || current->next != NULL) {

		tac_derefll_single(buffer, current, prev_type, ctx);

		prev_type = current->type;
		current = current->next;

		if (current == NULL) break;
	}

	derefll_dtor(dll);
}
