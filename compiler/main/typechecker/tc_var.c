#define _GNU_SOURCE

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

static bool tc_var_in_struct(struct Type* containing_type, struct Variable* member, struct TCCtx* tcctx) {

	char* struct_name = tc_get_underlying_struct_name(containing_type);

	assert(struct_name);

	char* member_name = member->simple_var->name;

	struct STST* stst = tcctx->st->stst;

	// check that access is be a member that actually exists on that structure
	if (!stst_has_member(stst, struct_name, member_name)) {
		char* snippet = NULL;
		asprintf(&snippet, "could not find member '%s' on struct '%s'", member_name, struct_name);
		error_snippet(tcctx, snippet, TC_ERR_VAR_NOT_FOUND);

		free(snippet);
		return false;
	}

	if (!tc_simplevar_in_struct(containing_type, member->simple_var, tcctx)) {
		return false;
	}

	if (member->member_access == NULL) {
		return true;
	}

	//struct STSTLine* line = stst_get(stst, struct_name);

	//TODO: apply all indices and then get that type

	struct MemberAccess ma = {
	    .structType = containing_type,
	    .member = member,
	};

	struct Type* type_after_indices = infer_in_context_once(tcctx->st, &ma);

	// continue checking recursively
	return tc_var_in_struct(type_after_indices, member->member_access, tcctx);

	//return true;
}

bool tc_var(struct Variable* v, struct TCCtx* tcctx) {

	struct SimpleVar* sv = v->simple_var;

	if (!tc_simplevar(sv, tcctx)) { return false; }

	struct Variable* member_access = v->member_access;

	if (member_access == NULL) {
		return true;
	}

	struct LVSTLine* line = lvst_get(tcctx->st->lvst, sv->name);

	return tc_var_in_struct(line->type, member_access, tcctx);
}
