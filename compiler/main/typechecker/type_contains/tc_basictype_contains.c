#include <stdbool.h>
#include <string.h>

#include "ast/ast.h"

#include "tc_type_contains.h"

bool tc_basictype_contains(struct BasicType* expect, struct Type* actual) {

	struct BasicType* actual_bt = actual->basic_type;

	if (expect->simple_type != NULL) {
		return tc_simpletype_contains(expect->simple_type, actual);
	}

	if (expect->subr_type != NULL && actual_bt->subr_type != NULL) {
		return tc_subrtype_contains(expect->subr_type, actual_bt->subr_type);
	}

	return false;
}
