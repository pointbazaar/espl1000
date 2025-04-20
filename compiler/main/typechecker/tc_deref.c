#define _GNU_SOURCE

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"

//Table Includes
#include "tables/symtable/symtable.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "tcctx.h"

#include "typeinference/typeinfer.h"
#include <stdlib.h>

bool tc_deref(struct Deref* d, struct TCCtx* tcctx) {

	// check that we can deref whatever it is
	struct Type* type = infer_type_term(tcctx->st, d->term);

	// has to be pointer type or array type
	if (!type->array_type && !type->pointer_type) {
		char* str_t1 = str_type(type);

		char* snippet = str_deref(d);

		char* msg;

		asprintf(&msg, "\tcannot deref type:%s", str_t1);

		error_snippet_and_msg(tcctx, snippet, msg, TC_ERR_CANNOT_DEREF_THIS);

		free(snippet);
		free(msg);

		return false;
	}

	// TODO: handle type param

	return tc_term(d->term, tcctx);
}
