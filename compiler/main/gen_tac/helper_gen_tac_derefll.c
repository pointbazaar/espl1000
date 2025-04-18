#include <assert.h>

#include "ast/ast_declare.h"
#include "ast/ast.h"

#include "cli/flags/flags.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "util/ctx.h"

#include "tables/symtable/symtable.h"
#include "tables/stst/stst.h"

#include "gen_tac.h"

#include "derefll/derefll.h"

#include "helper_gen_tac_derefll.h"

void tac_derefll_single(struct TACBuffer* buffer, struct DerefLL* dll, struct Type* prev_type, struct Ctx* ctx) {

	if (flags_debug(ctx_flags(ctx))) {
		printf("[debug] %s\n", __func__);
	}

	const bool x86 = flags_x86(ctx_flags(ctx));
	const uint8_t width = (x86) ? 8 : 2;

	switch (dll->action) {

		case DEREFLL_INIT: {
			char* name = dll->initial->name;
			struct LVST* lvst = ctx_tables(ctx)->lvst;
			struct SST* sst = ctx_tables(ctx)->sst;

			if (lvst_contains(lvst, name)) {
				const uint32_t local_index = lvst_index_of(lvst, name);
				tacbuffer_append(buffer, makeTACLoadLocalAddr(make_temp(), local_index, width));
			} else if (sst_contains(sst, name)) {
				const uint32_t index = sst_index_of(sst, name);
				tacbuffer_append(buffer, makeTACLoadFunctionPtr(make_temp(), index));
			} else {
				fprintf(stderr, "%s: %s not found in LVST or SST\n", __func__, dll->initial->name);
				assert(false);
			}
		} break;

		case DEREFLL_INDEX: {
			uint32_t tbase = tacbuffer_last_dest(buffer);

			tac_expr(buffer, dll->index_expr, ctx);
			uint32_t texpr = tacbuffer_last_dest(buffer);

			tacbuffer_append(buffer, makeTACBinOp(tbase, TAC_OP_ADD, texpr));
		} break;

		case DEREFLL_MEMBER: {
			//find member offset
			struct STST* stst = ctx_tables(ctx)->stst;

			struct BasicType* bt = prev_type->basic_type;

			if (!bt) {
				assert(prev_type->pointer_type);
				bt = prev_type->pointer_type->element_type->basic_type;
			}

			assert(bt);
			assert(bt->simple_type);
			assert(bt->simple_type->struct_type);

			char* struct_name = bt->simple_type->struct_type->type_name;
			uint32_t offset = stst_member_offset(stst, struct_name, dll->member_name, x86);

			//add that offset
			if (offset != 0) {
				const uint32_t tbase = tacbuffer_last_dest(buffer);
				const uint32_t tinc = make_temp();
				tacbuffer_append(buffer, makeTACConst(tinc, offset));
				tacbuffer_append(buffer, makeTACBinOp(tbase, TAC_OP_ADD, tinc));
			}
		} break;

		case DEREFLL_DEREF:
			// TODO: the width has to be specific to what is being dereferenced
			tacbuffer_append(buffer, makeTACLoad(make_temp(), tacbuffer_last_dest(buffer), width));
			break;
	}
}
