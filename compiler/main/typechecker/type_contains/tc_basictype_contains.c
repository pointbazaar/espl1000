#include <stdbool.h>
#include <string.h>

#include "ast/ast.h"

#include "tc_type_contains.h"

bool tc_basictype_contains(struct BasicType* expect, struct BasicType* actual) {

	if (expect->simple_type != NULL && actual->simple_type != NULL) {
		return tc_simpletype_contains(expect->simple_type, actual);
	}
	if (expect->subr_type != NULL && actual->subr_type != NULL) {
		return tc_subrtype_contains(expect->subr_type, actual->subr_type);
	}
	return false;
}
