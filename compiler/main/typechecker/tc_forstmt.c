#include <stdlib.h>
#include <stdio.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"

//Typeinference Includes
#include "compiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"
#include "tcctx.h"

bool tc_forstmt(struct ForStmt* f, struct TCCtx* tcctx) {

	if (tcctx->debug) {
		printf("[debug] typecheck for stmt\n");
	}

	tcctx->current_line_num = f->super.line_num;

	if (!tc_range(f->range, tcctx)) { return false; }

	tcctx->depth_inside_loop++;
	bool is_ok = tc_stmtblock(f->block, tcctx);
	tcctx->depth_inside_loop--;

	return is_ok;
}
