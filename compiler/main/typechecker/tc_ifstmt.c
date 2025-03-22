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

bool tc_ifstmt(struct IfStmt* i, struct TCCtx* tcctx) {

	if (tcctx->debug) {
		printf("[debug] typecheck if stmt\n");
	}

	tcctx->current_line_num = i->super.line_num;

	struct Type* type = infer_type_expr(tcctx->st, i->condition);

	if (!is_bool_type(type)) {

		char* s1 = str_expr(i->condition);

		char* msg;
		asprintf(&msg, "if %s {", s1);

		free(s1);

		error_snippet(tcctx, msg, TC_ERR_CONDITION_REQUIRES_BOOL);

		char* s2 = str_type(type);

		fprintf(stderr, "actual type: %s\n\n", s2);

		free(s2);

		free(msg);

		return false;
	}

	bool success = tc_stmtblock(i->block, tcctx);

	if (i->else_block != NULL) {
		success &= tc_stmtblock(i->else_block, tcctx);
	}

	return success;
}
