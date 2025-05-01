//AST Includes
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ast/ast.h"
#include "ast/util/str_ast.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"

#include "_tc.h"

#include "typeinference/typeinfer.h"
#include "tcctx.h"

#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"

static bool tc_simplevar_indices_must_be_int(struct SimpleVar* sv, struct TCCtx* tcctx) {

	if (sv->count_indices == 0) { return true; }

	//check that each index is of an integer type
	for (uint32_t i = 0; i < sv->count_indices; i++) {

		struct Expr* indexExpr = sv->indices[i];

		if (!tc_expr(indexExpr, tcctx)) { return false; }

		struct Type* type = infer_type_expr(tcctx->st, indexExpr);

		if (!type) {
			return false;
		}

		if (!is_integer_type(type)) {

			char* snippet = str_simple_var(sv);

			error_snippet(tcctx, snippet, TC_ERR_INDEX_NOT_INTEGER_TYPE);

			free(snippet);

			return false;
		}
	}

	return true;
}

static bool tc_simplevar_indices(struct Type* containing_type, struct SimpleVar* sv, struct TCCtx* tcctx) {

	if (!containing_type) { return false; }

	if (sv->count_indices == 0) { return true; }

	if (!tc_simplevar_indices_must_be_int(sv, tcctx)) {
		return false;
	}

	const char* struct_name = tc_get_underlying_struct_name(containing_type);

	struct Type* indexed_type = containing_type;

	if (struct_name) {

		// find the member type containing_type::sv->name
		struct Type* member_type = stst_member_type(tcctx->st->stst, struct_name, sv->name);

		if (member_type) {
			indexed_type = member_type;
		}
	}

	//check that the correct number of indices was used
	//(meaning not too many)

	const uint32_t max_indices = max_indices_allowed(indexed_type);

	if (sv->count_indices > max_indices) {

		char* snippet = str_simple_var(sv);

		error_snippet(tcctx, snippet, TC_ERR_TOO_MANY_INDICES);

		free(snippet);

		return false;
	}

	return true;
}

bool tc_simplevar_in_struct(struct Type* containing_type, struct SimpleVar* sv, struct TCCtx* tcctx) {
	if (!containing_type || !sv || !tcctx) {
		return false;
	}

	char* struct_name = tc_get_underlying_struct_name(containing_type);

	if (!struct_name) {
		return false;
	}

	if (!stst_has_member(tcctx->st->stst, struct_name, sv->name)) {

		char* snippet = str_simple_var(sv);

		error_snippet(tcctx, snippet, TC_ERR_VAR_NOT_FOUND);

		free(snippet);

		return false;
	}

	return tc_simplevar_indices(containing_type, sv, tcctx);
}

bool tc_simplevar(struct SimpleVar* sv, struct TCCtx* tcctx) {

	//does it even exist in the symbol table?
	char* name = sv->name;

	bool in_lvst = lvst_contains(tcctx->st->lvst, name);
	bool in_sst = sst_contains(tcctx->st->sst, name);

	if (!in_lvst && !in_sst) {

		char* snippet = str_simple_var(sv);

		error_snippet(tcctx, snippet, TC_ERR_VAR_NOT_FOUND);

		free(snippet);

		return false;
	}

	if (!in_lvst && sv->count_indices > 0) {
		error(tcctx, "cannot use indices for something thats not a local var/arg", TC_ERR_CANNOT_INDEX_INTO);
		return false;
	}

	if (in_lvst) {
		struct LVSTLine* line = lvst_get(tcctx->st->lvst, name);

		return tc_simplevar_indices(line->type, sv, tcctx);
	}

	// only in sst
	return true;
}
