
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

		case STMT_KIND_IF: return tc_ifstmt(s->ptr.if_stmt, tcctx, true);
		case STMT_KIND_RETURN: return tc_retstmt(s->ptr.return_stmt, tcctx);
		case STMT_KIND_CALL:
		case STMT_KIND_WHILE:
		case STMT_KIND_ASSIGN:
		case STMT_KIND_FOR:
		case STMT_KIND_LOCAL_VAR_DECL:
		case STMT_KIND_BREAK:
		case STMT_KIND_CONTINUE: {
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

		case STMT_KIND_CALL: return tc_methodcall(s->ptr.call, tcctx);
		case STMT_KIND_WHILE: return tc_whilestmt(s->ptr.while_stmt, tcctx);
		case STMT_KIND_IF: return tc_ifstmt(s->ptr.if_stmt, tcctx, false);
		case STMT_KIND_RETURN: return tc_retstmt(s->ptr.return_stmt, tcctx);
		case STMT_KIND_ASSIGN: return tc_assignstmt(s->ptr.assign_stmt, tcctx);
		case STMT_KIND_FOR: return tc_forstmt(s->ptr.for_stmt, tcctx);
		case STMT_KIND_LOCAL_VAR_DECL: return tc_local_var_decl_stmt(s->ptr.local_var_decl_stmt, tcctx);
		case STMT_KIND_BREAK:
		case STMT_KIND_CONTINUE:
			return true;

		default:
			// invalid stmt
			return false;
	}
	return true;
}
