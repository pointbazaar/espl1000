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

void tac_simplevar(struct TACBuffer* buffer, struct SimpleVar* v, struct Ctx* ctx){

	//TODO: remove special case
	//special case for the simplest case... just a temp fix bc
	//most of the codegen is not ready to emit code for register pairs just yet.

	if(v->count_indices == 0){

		struct LVST* lvst = ctx_tables(ctx)->lvst;
		uint32_t offset = lvst_index_of(lvst, v->name);
		tacbuffer_append(buffer, makeTACLoadLocal(make_temp(), offset));

		return;
	}

	tac_simplevar_addr(buffer, v, ctx);

	uint32_t tlast = tacbuffer_last_dest(buffer);

	tacbuffer_append(buffer, makeTACLoad(make_temp(), tlast));
}

void tac_simplevar_addr(struct TACBuffer* buffer, struct SimpleVar* sv, struct Ctx* ctx){

	struct DerefLL* dll = derefll_ctor_simplevar(sv);

	derefll_annotate_types(dll, ctx, NULL);

	//now iterate over the linked list and emit the correct TACs along the way

	struct DerefLL* current = dll;
	struct Type* prev_type = NULL;

	while(current->next != NULL || current == dll){

		tac_derefll_single(buffer, current, prev_type, ctx);

		prev_type = current->type;
		current = current->next;
	}

	derefll_dtor(dll);
}

