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

bool tc_whilestmt(struct WhileStmt* w, struct TCCtx* tcctx) {

	tcctx->current_line_num = w->super.line_num;

	struct Type* type = infer_type_expr(tcctx->st, w->condition);

	if (!is_bool_type(type)) {

		char* s1 = str_expr(w->condition);

		char* msg;
		asprintf(&msg, "while %s {", s1);

		free(s1);

		error_snippet(tcctx, msg, TC_ERR_CONDITION_REQUIRES_BOOL);

		free(msg);

		return false;
	}

	tcctx->depth_inside_loop++;
	bool has_err = tc_stmtblock(w->block, tcctx);
	tcctx->depth_inside_loop--;

	return has_err;
}
