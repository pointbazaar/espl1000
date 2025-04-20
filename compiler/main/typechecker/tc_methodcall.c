#define _GNU_SOURCE

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <typechecker/type_contains/tc_type_contains.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"
#include "ast/util/equals_ast.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

//Typeinference Includes
#include "compiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"
#include "typecheck.h"
#include "tcctx.h"

static bool tc_methodcall_args(
    struct Call* m,
    struct Type** expect_types,
    uint8_t expect_args,
    struct TCCtx* tcctx);

static bool tc_methodcall_arg(
    struct Call* m,
    struct Type* expect_type,
    struct Expr* actual_expr,
    struct TCCtx* tcctx);

bool tc_methodcall(struct Call* m, struct TCCtx* tcctx) {

	if (tcctx->debug) {
		printf("[debug] typecheck methodcall\n");
	}

	tcctx->current_line_num = m->super.line_num;

	if (!tc_var(m->callable, tcctx)) { return false; }

	struct Type* subrtype = infer_type_variable(tcctx->st, m->callable);

	if (subrtype->basic_type == NULL || subrtype->basic_type->subr_type == NULL) {

		char* snippet = str_call(m);
		error_snippet(tcctx, snippet, TC_ERR_LOCAL_VAR_NOT_A_SUBROUTINE);
		free(snippet);
		return false;
	}

	struct Type** expect_types = NULL;
	uint8_t expect_args = 0;

	expect_types = subrtype->basic_type->subr_type->arg_types;
	expect_args = subrtype->basic_type->subr_type->count_arg_types;

	if (!tcctx->current_fn->decl->has_side_effects && subrtype->basic_type->subr_type->has_side_effects) {
		//method with side effects called
		//in method marked as without side effects
		error(tcctx, "called subr with side-effects in pure subr", TC_ERR_SIDE_EFFECT_IN_PURE_CONTEXT);
		return false;
	}

	bool err1 = tc_methodcall_args(m, expect_types, expect_args, tcctx);

	return err1;
}

static bool tc_methodcall_args(
    struct Call* m,
    struct Type** expect_types,
    uint8_t expect_args,
    struct TCCtx* tcctx) {

	if (tcctx->debug) {
		printf("[debug] typecheck methodcall args\n");
	}

	const uint8_t actual_args = m->count_args;

	if (actual_args != expect_args) {

		char* snippet = str_call(m);

		char* msg;
		asprintf(&msg, "expected: %d args", expect_args);

		error_snippet_and_msg(tcctx, snippet, msg, TC_ERR_ARG_NUM_MISMATCH);

		free(snippet);
		free(msg);
		return false;
	}

	for (uint8_t i = 0; i < expect_args; i++) {

		struct Type* expect_type = expect_types[i];

		struct Expr* actual_expr = m->args[i];

		if (!tc_methodcall_arg(m, expect_type, actual_expr, tcctx)) { return false; }
	}
	return true;
}

static bool tc_methodcall_arg(
    struct Call* m,
    struct Type* expect_type,
    struct Expr* actual_expr,
    struct TCCtx* tcctx) {

	struct Type* actual_type = infer_type_expr(tcctx->st, actual_expr);

	assert(expect_type != NULL);
	assert(actual_type != NULL);

	//TODO: we can get rid of the integer special cases maybe
	//thansk to tc_type_contains
	if (is_integer_type(expect_type) && is_integer_type(actual_type)) { return true; }

	//TODO: deal with the type parameter binding,
	//meaning calling list_add(List<?T0> list, ?T0 elem)
	//with list of type List<int> must bind ?T0 := int
	//and typecheck elem to be of type int.
	if (!tc_type_contains(expect_type, actual_type)) {

		char* snippet = str_call(m);
		char* s2 = str_expr(actual_expr);

		char* sTypeActual = str_type(actual_type);
		char* sTypeExpected = str_type(expect_type);

		char* msg;
		asprintf(&msg, "%s, (of type %s), but expected type %s", s2, sTypeActual, sTypeExpected);

		free(s2);
		free(sTypeActual);
		free(sTypeExpected);

		error_snippet_and_msg(tcctx, snippet, msg, TC_ERR_ARG_TYPE_MISMATCH);

		free(snippet);
		free(msg);

		return false;
	}

	if (!tc_expr(actual_expr, tcctx)) { return false; }

	return true;
}
