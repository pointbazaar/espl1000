#include "ast/ast_declare.h"
#include "ast/ast.h"

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "util/ctx.h"

#include "tables/symtable/symtable.h"
#include "tables/stst/stst.h"

#include "gen_tac.h"

#include "derefll/derefll.h"

#include "helper_gen_tac_derefll.h"

void tac_derefll_single(struct TACBuffer* buffer, struct DerefLL* dll, struct Type* prev_type, struct Ctx* ctx) {

	switch (dll->action) {

		case DEREFLL_INIT: {
			struct LVST* lvst = ctx_tables(ctx)->lvst;
			uint32_t local_index = lvst_index_of(lvst, dll->initial->name);
			tacbuffer_append(buffer, makeTACLoadLocalAddr(make_temp(), local_index));
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
			char* struct_name = prev_type->basic_type->simple_type->struct_type->type_name;
			uint32_t offset = stst_member_offset(stst, struct_name, dll->member_name);

			//add that offset
			if (offset != 0)
				tacbuffer_append(buffer, makeTACBinOpImmediate(tacbuffer_last_dest(buffer), TAC_OP_ADD, offset));
		} break;

		case DEREFLL_DEREF:
			// TODO: the width there needs to be architecture specific, for x86 it's not 2 but 8 bytes
			tacbuffer_append(buffer, makeTACLoad(make_temp(), tacbuffer_last_dest(buffer), 2));
			break;
	}
}
