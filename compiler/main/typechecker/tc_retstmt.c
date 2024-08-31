#include <stdlib.h>
#include <stdio.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"
#include "ast/util/equals_ast.h"

//Typeinference Includes
#include "compiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"
#include "tcctx.h"

bool tc_retstmt(struct RetStmt* r, struct TCCtx* tcctx) {

	tcctx->current_line_num = r->super.line_num;

	struct Type* returnType = tcctx->current_fn->decl->return_type;

	if (!tc_expr(r->return_value, tcctx)) { return false; }

	struct Type* returnedType = infer_type_expr(tcctx->st, r->return_value);

	if (is_integer_type(returnType) && is_integer_type(returnedType)) { return true; }

	if (!eq_type(returnType, returnedType)) {

		char* s1 = str_type(returnType);
		char* s2 = str_type(returnedType);
		char* snippet = str_ret_stmt(r);

		char* msg;
		asprintf(&msg, "expected type: %s, actual type: %s", s1, s2);

		free(s1);
		free(s2);

		error_snippet_and_msg(tcctx, snippet, msg, TC_ERR_WRONG_RETURN_TYPE);
		free(snippet);
		free(msg);

		return false;
	}

	return true;
}
