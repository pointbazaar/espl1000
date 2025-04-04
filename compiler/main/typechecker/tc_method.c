
#include <analyzer/lv/lv_analyzer.h>

//AST Includes
#include "ast/ast.h"

//Table Includes
#include "tables/lvst/lvst.h"
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

//Typechecker Includes
#include "_tc.h"
#include "typecheck.h"
#include "tcctx.h"

bool tc_method(struct Method* m, struct TCCtx* tcctx) {

	if (tcctx->debug) {
		printf("[debug] typecheck method: %s\n", m->decl->name);
	}

	tcctx->current_line_num = m->super.line_num;

	tcctx->current_fn = m;

	lvst_clear(tcctx->st->lvst);
	lvst_fill(m, tcctx->st);

	return tc_stmtblock(m->block, tcctx, true);
}
