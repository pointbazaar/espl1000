#include <stdbool.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/util/equals_ast.h"
#include "typechecker/util/tc_utils.h"

#include "tc_type_contains.h"

bool tc_pointer_type_contains(struct PointerType* expect, struct Type* actual) {

	// integers can be used as pointers
	if (actual->pointer_type == NULL) {

		if (actual->array_type != NULL) {
			return eq_type(expect->element_type, actual->array_type->element_type);
		}

		return is_integer_type(actual);
	}

	struct PointerType* apt = actual->pointer_type;

	// pointers of same underlying type can be assigned to each other
	return tc_type_contains(expect->element_type, apt->element_type);
}
