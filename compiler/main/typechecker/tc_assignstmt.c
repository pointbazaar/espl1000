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
#include "tables/symtable/symtable.h"

//Typeinference Includes
#include "compiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"
#include "typecheck.h"
#include "tcctx.h"

static bool check_type_rules_assign(struct AssignStmt* a, struct TCCtx* tcctx);

bool tc_assignstmt(struct AssignStmt* a, struct TCCtx* tcctx) {

	if (tcctx->debug) {
		printf("[debug] typecheck assign stmt\n");
	}

	tcctx->current_line_num = a->super.line_num;

	assert(tcctx != NULL);
	assert(tcctx->st != NULL);
	assert(tcctx->st->lvst != NULL);
	assert(a->lvalue);

	char* name;
	if (a->lvalue->var) {

		assert(a->lvalue->var->simple_var != NULL);
		assert(a->lvalue->var->simple_var->name != NULL);

		name = a->lvalue->var->simple_var->name;
	} else {
		assert(a->lvalue->deref);
		struct Term* term = a->lvalue->deref->term;
		assert(term->kind == TERM_KIND_VAR);
		name = term->ptr.var_term->simple_var->name;
	}

	struct LVSTLine* line = lvst_get(tcctx->st->lvst, name);

	if (!line) {
		return false;
	}

	if (line->read_only) {
		error(tcctx, "variable can only be read but not written to.", TC_ERR_VAR_READONLY);
	}

	if (a->lvalue->var) {
		if (!tc_var(a->lvalue->var, tcctx)) { return false; }
	}
	if (a->lvalue->deref) {
		if (!tc_deref(a->lvalue->deref, tcctx)) { return false; }
	}

	if (!tc_expr(a->expr, tcctx)) { return false; }

	return check_type_rules_assign(a, tcctx);
}

static bool check_type_rules_assign(struct AssignStmt* a, struct TCCtx* tcctx) {

	if (tcctx->debug) {
		printf("[debug] typecheck assignment rules\n");
	}

	struct Type* right = infer_type_expr(tcctx->st, a->expr);

	struct Type* left = a->opt_type;

	if (a->opt_type == NULL) {
		left = infer_type_lvalue(tcctx->st, a->lvalue);
	}

	if (left == NULL) {
		return false;
	}

	if (is_integer_type(left) && is_integer_type(right)) { return true; }

	if (!tc_type_contains(left, right)) {

		char* str_t1 = str_type(left);
		char* str_t2 = str_type(right);

		char* snippet = str_assign_stmt(a);

		char* msg;

		asprintf(&msg, "\texpected type: %s, actual type: %s", str_t1, str_t2);

		error_snippet_and_msg(tcctx, snippet, msg, TC_ERR_ASSIGN_TYPE_MISMATCH);

		free(str_t1);
		free(str_t2);
		free(snippet);
		free(msg);

		return false;
	}
	return true;
}
