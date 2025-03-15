#include <stdbool.h>

#include "ast/ast.h"

#include "tc_type_contains.h"

bool tc_subrtype_contains(struct SubrType* expect, struct SubrType* actual) {

	if (!tc_type_contains(expect->return_type, actual->return_type)) { return false; }
	if (actual->has_side_effects && !expect->has_side_effects) { return false; }

	if (expect->count_arg_types != actual->count_arg_types) { return false; }

	for (int i = 0; i < expect->count_arg_types; ++i) {
		struct Type* ei = expect->arg_types[i];
		struct Type* ai = actual->arg_types[i];
		if (!tc_type_contains(ei, ai)) { return false; }
	}
	return true;
}
