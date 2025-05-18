#include <stdio.h>
#include <stdlib.h>

#include "cli/flags/flags.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "gen_tac.h"

bool tac_forstmt(struct TACBuffer* buffer, struct ForStmt* f, struct Ctx* ctx) {

	//t1 = f->range->start
	//f->index_name = t1

	//L0:
	//t2 = f->range->end
	//t3 = f->index_name
	//if-cmp-goto t2 >= t3 L1

	//goto Lend:

	//L1:
	// block
	// t3 = f->index_name
	// t3++
	// goto L0
	//Lend:

	//f->index_name
	//f->range->start
	//f->range->end

	uint32_t l0 = make_label();
	uint32_t l1 = make_label();
	uint32_t lend = make_label();

	struct LVST* lvst = ctx_tables(ctx)->lvst;
	const bool x86 = flags_x86(ctx_flags(ctx));
	const uint8_t addr_width = (x86) ? 8 : 2;
	const uint8_t local_width = lvst_sizeof_var(lvst, f->index_name, x86);
	const uint32_t local_index = lvst_index_of(lvst, f->index_name);

	ctx_enter_loop(ctx, l0, lend);

	uint32_t t3 = make_temp();
	//char t3str[10];
	//sprintf(t3str, "t%d", t3);

	if (!tac_expr(buffer, f->range->start, ctx)) {
		return false;
	}
	uint32_t t1 = tacbuffer_last_dest(buffer);
	tacbuffer_append(buffer, makeTACStoreLocal(f->super.line_num, local_index, t1));

	//L0:
	tacbuffer_append(buffer, makeTACLabel(f->super.line_num, l0));

	if (!tac_expr(buffer, f->range->end, ctx)) {
		return false;
	}
	uint32_t t2 = tacbuffer_last_dest(buffer);

	tacbuffer_append(buffer, makeTACLoadLocalAddr(f->super.line_num, make_temp(), local_index, addr_width));
	tacbuffer_append(buffer, makeTACLoad(f->super.line_num, t3, tacbuffer_last_dest(buffer), local_width));

	tacbuffer_append(buffer, makeTACIfCMPGoto(f->super.line_num, t2, TAC_OP_CMP_GE, t3, l1));

	tacbuffer_append(buffer, makeTACGoto(f->super.line_num, lend));

	//L1:
	tacbuffer_append(buffer, makeTACLabel(f->super.line_num, l1));

	if (!tac_stmtblock(buffer, f->block, ctx)) {
		return false;
	}

	// t3++
	tacbuffer_append(buffer, makeTACLoadLocalAddr(f->super.line_num, make_temp(), local_index, addr_width));
	tacbuffer_append(buffer, makeTACLoad(f->super.line_num, t3, tacbuffer_last_dest(buffer), local_width));

	const uint32_t tinc = make_temp();
	tacbuffer_append(buffer, makeTACConst(f->super.line_num, tinc, 1));
	tacbuffer_append(buffer, makeTACBinOp(f->super.line_num, t3, TAC_OP_ADD, tinc));
	tacbuffer_append(buffer, makeTACStoreLocal(f->super.line_num, local_index, t3));

	tacbuffer_append(buffer, makeTACGoto(f->super.line_num, l0));

	//Lend:
	tacbuffer_append(buffer, makeTACLabel(f->super.line_num, lend));

	return ctx_exit_loop(ctx);
}
