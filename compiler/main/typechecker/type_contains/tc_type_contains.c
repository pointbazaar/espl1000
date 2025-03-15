#include <stdbool.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/util/equals_ast.h"
#include "typechecker/util/tc_utils.h"

#include "tc_type_contains.h"

bool tc_type_contains(struct Type* expect, struct Type* actual) {

	if (expect->pointer_type != NULL) {

		return tc_pointer_type_contains(expect->pointer_type, actual);
	}

	if (expect->basic_type != NULL && actual->basic_type != NULL) {

		return tc_basictype_contains(expect->basic_type, actual->basic_type);
	}

	if (expect->array_type != NULL) {

		if (actual->array_type != NULL) {

			return tc_arraytype_contains(expect->array_type, actual->array_type);
		} else {

			//allow integer assignments to array types
			if (actual->basic_type != NULL && actual->basic_type->simple_type != NULL && actual->basic_type->simple_type->primitive_type != NULL) {
				struct PrimitiveType* pt = actual->basic_type->simple_type->primitive_type;

				return pt->is_int_type;
			}

			return false;
		}
	}

	//so expect->m2 should be != NULL
	//meaning that anything on the right is ok, since it's a type parameter
	return expect->type_param != NULL;
}
