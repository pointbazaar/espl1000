
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

bool tc_address_of(struct AddressOf* ao, struct TCCtx* tcctx) {

	// check if we can take the address of whatever that is

	// check that we can deref whatever it is
	struct Type* type = infer_type_term(tcctx->st, ao->term);

	if (type->basic_type && type->basic_type->subr_type) {
		char* snippet = str_address_of(ao);
		char* str_t1 = str_type(type);
		char* msg;
		asprintf(&msg, "\tcannot deref subroutine type:%s", str_t1);

		error_snippet_and_msg(tcctx, snippet, msg, TC_ERR_CANNOT_DEREF_THIS);

		free(snippet);
		free(msg);

		return false;
	}

	// TODO: check that whatever that is, that it is something
	// which we can actually get the address of.

	if (type->basic_type && type->basic_type->simple_type) {
		return tc_term(ao->term, tcctx);
	}

	if (type->pointer_type) {
		return tc_term(ao->term, tcctx);
	}

	// TODO: handle the other types.
	// right now, just error out here

	char* str_t1 = str_type(type);

	char* snippet = str_address_of(ao);

	char* msg;

	asprintf(&msg, "\tcannot deref type:%s", str_t1);

	error_snippet_and_msg(tcctx, snippet, msg, TC_ERR_CANNOT_DEREF_THIS);

	free(snippet);
	free(msg);

	return false;

	// TODO: handle type param

	return tc_term(ao->term, tcctx);
}
