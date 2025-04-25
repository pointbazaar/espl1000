#define _GNU_SOURCE

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

	const bool tc_ok = tc_stmtblock(m->block, tcctx, true);

	if (tcctx->debug && !tc_ok) {
		char* lvst_filename = NULL;
		asprintf(&lvst_filename, "%s.lvst", m->decl->name);

		if (!lvst_filename) {
			fprintf(stderr, "could not allocate filename for lvst\n");
			return false;
		}

		lvst_print_filename(tcctx->st->lvst, lvst_filename);
	}

	return tc_ok;
}
