
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
#include <stdio.h>
#include <stdlib.h>

bool tc_stmt_must_return(struct Stmt* s, struct TCCtx* tcctx) {

	switch (s->kind) {

		case 3: return tc_ifstmt(s->ptr.m3, tcctx, true);
		case 4: return tc_retstmt(s->ptr.m4, tcctx);
		case 1:
		case 2:
		case 5:
		case 7:
		case 10:
		case 99: {
			char* snippet = str_stmt(s);
			error_snippet_and_msg(tcctx, snippet, "should return here", TC_ERR_MUST_RETURN);
			free(snippet);
			return false;
		}

		default:
			// invalid stmt
			return false;
	}
}

bool tc_stmt(struct Stmt* s, struct TCCtx* tcctx, bool must_return) {

	tcctx->current_line_num = s->super.line_num;

	if (tcctx->debug) {
		printf("[debug] typecheck statement line %d\n", tcctx->current_line_num);
	}

	if (must_return) {
		return tc_stmt_must_return(s, tcctx);
	}

	switch (s->kind) {

		case 1: return tc_methodcall(s->ptr.m1, tcctx);
		case 2: return tc_whilestmt(s->ptr.m2, tcctx);
		case 3: return tc_ifstmt(s->ptr.m3, tcctx, false);
		case 4: return tc_retstmt(s->ptr.m4, tcctx);
		case 5: return tc_assignstmt(s->ptr.m5, tcctx);
		case 7: return tc_forstmt(s->ptr.m7, tcctx);
		case 10: return tc_local_var_decl_stmt(s->ptr.m10, tcctx);

		case 99:
			if (s->is_continue) {}
			if (s->is_break) {}

		default:
			// invalid stmt
			return false;
	}
	return true;
}
